/*
 * Copyright (C) 2004-2024 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "economy/economy.h"

#include <algorithm>
#include <memory>

#include "base/log.h"
#include "base/macros.h"
#include "base/wexception.h"
#include "economy/cmd_call_economy_balance.h"
#include "economy/flag.h"
#include "economy/request.h"
#include "economy/route.h"
#include "economy/routeastar.h"
#include "economy/router.h"
#include "economy/warehousesupply.h"
#include "logic/game.h"
#include "logic/map_objects/tribes/soldier.h"
#include "logic/map_objects/tribes/tribe_descr.h"
#include "logic/map_objects/tribes/warehouse.h"
#include "logic/player.h"

namespace Widelands {

Serial Economy::last_economy_serial_ = 0;

void Economy::initialize_serial() {
	verb_log_info("Initializing economy serial\n");
	last_economy_serial_ = 0;
}

Economy::Economy(Player& player, WareWorker wwtype)
   : Economy(player, last_economy_serial_++, wwtype) {
}

Economy::Economy(Player& player, Serial init_serial, WareWorker wwtype)
   : serial_(init_serial),
     owner_(player),
     type_(wwtype),
     request_timerid_(0),
     options_window_(nullptr) {
	last_economy_serial_ = std::max(last_economy_serial_, serial_ + 1);
	const TribeDescr& tribe = player.tribe();
	DescriptionIndex const nr_wares_or_workers = wwtype == wwWARE ?
                                                   player.egbase().descriptions().nr_wares() :
                                                   player.egbase().descriptions().nr_workers();
	wares_or_workers_.set_nrwares(nr_wares_or_workers);

	target_quantities_ = new TargetQuantity[nr_wares_or_workers];
	for (DescriptionIndex i = 0; i < nr_wares_or_workers; ++i) {
		TargetQuantity tq;
		switch (type_) {
		case wwWARE:
			if (tribe.has_ware(i)) {
				tq.permanent = tribe.get_ware_descr(i)->default_target_quantity(tribe.name());
			} else {
				tq.permanent = 0;
			}
			break;
		case wwWORKER:
			tq.permanent = tribe.get_worker_descr(i)->default_target_quantity();
			break;
		}
		tq.last_modified = Time(0);
		target_quantities_[i] = tq;
	}

	router_.reset(new Router([this]() { reset_all_pathfinding_cycles(); }));
}

Economy::~Economy() {
	Notifications::publish(NoteEconomy{serial_, serial_, NoteEconomy::Action::kDeleted});

	if (!requests_.empty()) {
		log_warn("Economy still has requests left on destruction\n");
	}
	if (!flags_.empty()) {
		log_warn("Economy still has flags left on destruction\n");
	}
	if (!warehouses_.empty()) {
		log_warn("Economy still has warehouses left on destruction\n");
	}

	delete[] target_quantities_;
}

/**
 * \return an arbitrary flag in this economy.
 */
Flag* Economy::get_arbitrary_flag(const Economy* other) {
	if (flags_.empty()) {
		return nullptr;
	}

	if (other == nullptr) {
		return flags_[0];
	}

	for (Flag* f : flags_) {
		if (f->get_economy(other->type()) == other) {
			return f;
		}
	}
	return nullptr;
}

/**
 * Two flags have been connected; check whether their economies should be
 * merged.
 * Since we could merge into both directions, we preserve the economy that is
 * currently bigger (should be more efficient).
 */
void Economy::check_merge(const Flag& f1, const Flag& f2, WareWorker type) {
	Economy* e1 = f1.get_economy(type);
	Economy* e2 = f2.get_economy(type);
	if (e1 != e2) {
		if (e1->get_nrflags() < e2->get_nrflags()) {
			std::swap(e1, e2);
		}
		e1->merge(*e2);
	}
}

/**
 * Notify the economy that there may no longer be a connection between
 * the given flags in the road and seafaring network.
 */
void Economy::check_split(Flag& f1, Flag& f2, WareWorker type) {
	assert(&f1 != &f2);
	assert(f1.get_economy(type) == f2.get_economy(type));

	Economy* e = f1.get_economy(type);
	// No economy in the editor.
	if (e == nullptr) {
		return;
	}

	e->split_checks_.emplace_back(OPtr<Flag>(&f1), OPtr<Flag>(&f2));
	e->rebalance_supply();  // the real split-checking is done during rebalance
}

void Economy::check_splits() {
	EditorGameBase& egbase = owner().egbase();
	while (!split_checks_.empty()) {
		Flag* f1 = split_checks_.back().first.get(egbase);
		Flag* f2 = split_checks_.back().second.get(egbase);
		split_checks_.pop_back();

		if ((f1 == nullptr) || (f2 == nullptr)) {
			if ((f1 == nullptr) && (f2 == nullptr)) {
				continue;
			}
			if (f1 == nullptr) {
				f1 = f2;
			}
			if (f1->get_economy(type_) != this) {
				continue;
			}

			// Handle the case when two or more roads are removed simultaneously
			RouteAStar<AStarZeroEstimator> astar(*router_, type_, AStarZeroEstimator());
			astar.push(*f1);
			std::set<OPtr<Flag>> reachable;
			while (RoutingNode* current = astar.step()) {
				reachable.insert(&current->base_flag());
			}
			if (reachable.size() != flags_.size()) {
				split(reachable);
			}
			continue;
		}

		// If one (or both) of the flags have already been split off, we do not need to re-check
		if (f1->get_economy(type_) != this || f2->get_economy(type_) != this) {
			continue;
		}

		// Start an A-star searches from f1 with a heuristic bias towards f2,
		// because we do not need to do anything if f1 is still connected to f2.
		// If f2 is not reached by the search, split off all the nodes that have been
		// reached from f1. These nodes induce a connected subgraph.
		// This means that the newly created economy, which contains all the
		// flags that have been split, is already connected.
		RouteAStar<AStarEstimator> astar(*router_, type_, AStarEstimator(*egbase.mutable_map(), *f2));
		astar.push(*f1);
		std::set<OPtr<Flag>> reachable;

		for (;;) {
			RoutingNode* current = astar.step();
			if (current == nullptr) {
				split(reachable);
				break;
			}
			if (current == f2) {
				break;
			}
			reachable.insert(&current->base_flag());
		}
	}
}

/**
 * Calculate a route between two flags.
 *
 * This functionality has been moved to Router(). This is currently
 * merely a delegator.
 */
bool Economy::find_route(Flag& start, Flag& end, Route* const route, int32_t const cost_cutoff) {
	assert(start.get_economy(type_) == this);
	assert(end.get_economy(type_) == this);
	return router_->find_route(
	   start, end, route, type_, cost_cutoff, *owner().egbase().mutable_map());
}

struct ZeroEstimator {
	int32_t operator()(RoutingNode& /* node */) const {
		return 0;
	}
};

/**
 * Find the warehouse closest to the given starting flag.
 *
 * If the search was successful and \p route is non-null,
 * a route is also computed.
 *
 * \param start starting flag
 * \param route if non-null, fill in a route to the warehouse
 * \param cost_cutoff if positive, find paths of at most
 * that length (in milliseconds)
 */
Warehouse* Economy::find_closest_warehouse(Flag& start,
                                           Route* route,
                                           uint32_t cost_cutoff,
                                           const Economy::WarehouseAcceptFn& acceptfn) {
	if (warehouses().empty()) {
		return nullptr;
	}

	// A-star with zero estimator = Dijkstra
	RouteAStar<ZeroEstimator> astar(*router_, type_);
	astar.push(start);

	while (RoutingNode* current = astar.step()) {
		if ((cost_cutoff != 0u) &&
		    (type_ == wwWARE ? current->mpf_realcost_ware : current->mpf_realcost_worker) >
		       static_cast<int32_t>(cost_cutoff)) {
			return nullptr;
		}

		Flag& flag = current->base_flag();
		if (upcast(Warehouse, warehouse, flag.get_building())) {
			if (!acceptfn || acceptfn(*warehouse)) {
				if (route != nullptr) {
					astar.routeto(flag, *route);
				}
				return warehouse;
			}
		}
	}

	return nullptr;
}

/**
 * Add a flag to the flag array.
 * Only call from Flag init and split/merger code!
 */
void Economy::add_flag(Flag& flag) {
	assert(flag.get_economy(type_) == nullptr);

	flags_.push_back(&flag);
	flag.set_economy(this, type_);

	flag.reset_path_finding_cycle(type_);
}

/**
 * Remove a flag from the flag array.
 * Only call from Flag cleanup and split/merger code!
 */
void Economy::remove_flag(Flag& flag) {
	assert(flag.get_economy(type_) == this);

	do_remove_flag(flag);

	// automatically delete the economy when it becomes empty.
	if (flags_.empty()) {
		owner_.remove_economy(serial_);
	}
}

/**
 * Remove the flag, but don't delete the economy automatically.
 * This is called from the merge code.
 */
void Economy::do_remove_flag(Flag& flag) {
	flag.set_economy(nullptr, type_);

	// fast remove
	for (Flags::iterator flag_iter = flags_.begin(); flag_iter != flags_.end(); ++flag_iter) {
		if (*flag_iter == &flag) {
			*flag_iter = *(flags_.end() - 1);
			flags_.pop_back();
			return;
		}
	}
	throw wexception("trying to remove nonexistent flag");
}

/**
 * Callback for the incredibly rare case that the \ref Router pathfinding
 * cycle wraps around.
 */
void Economy::reset_all_pathfinding_cycles() {
	for (Flag* flag : flags_) {
		flag->reset_path_finding_cycle(type_);
	}
}

/**
 * Set the target quantities for the given DescriptionIndex to the
 * numbers given in permanent. Also update the last
 * modification time.
 *
 * This is called from Cmd_ResetTargetQuantity and Cmd_SetTargetQuantity
 */
void Economy::set_target_quantity(WareWorker economy_type,
                                  DescriptionIndex const ware_or_worker_type,
                                  Quantity const permanent,
                                  Time const mod_time) {
	assert(economy_type == type_);
	// Skip in release builds to get the most reasonable game state
	if (economy_type != type_) {
		log_warn_time(mod_time, "Economy type mismatch in set_target_quantity, skipping\n");
		return;
	}
#ifndef NDEBUG
	if (type_ == wwWARE) {
		assert(owner().egbase().descriptions().ware_exists(ware_or_worker_type));
	} else {
		assert(owner().egbase().descriptions().worker_exists(ware_or_worker_type));
	}
#endif
	TargetQuantity& tq = target_quantities_[ware_or_worker_type];
	tq.permanent = permanent;
	tq.last_modified = mod_time;
}

/**
 * Call this whenever some entity created a ware, e.g. when a lumberjack
 * has felled a tree.
 * This is also called when a ware is added to the economy through trade or
 * a merger.
 * Also notifies the corresponding other-type economy, if desired,
 * so it may check e.g. whether a worker for whom a tool was missing can now be created.
 */
void Economy::add_wares_or_workers(DescriptionIndex const id,
                                   Quantity const count,
                                   Economy* other_economy) {
	wares_or_workers_.add(id, count);
	start_request_timer();
	if (other_economy != nullptr) {
		assert(other_economy->type() != type_);
		other_economy->start_request_timer();
	}

	// TODO(unknown): add to global player inventory?
}

/**
 * Call this whenever a ware is destroyed or consumed, e.g. food has been
 * eaten or a warehouse has been destroyed.
 * This is also called when a ware is removed from the economy through trade or
 * a split of the Economy.
 */
void Economy::remove_wares_or_workers(DescriptionIndex const id, Quantity const count) {
#ifndef NDEBUG
	if (type_ == wwWARE) {
		assert(owner().egbase().descriptions().ware_exists(id));
	} else {
		assert(owner().egbase().descriptions().worker_exists(id));
	}
#endif
	wares_or_workers_.remove(id, count);

	// TODO(unknown): remove from global player inventory?
}

/**
 * Add the warehouse to our list of warehouses.
 * This also adds the wares in the warehouse to the economy. However, if wares
 * are added to the warehouse in the future, add_wares() must be called.
 */
void Economy::add_warehouse(Warehouse& wh) {
	warehouses_.push_back(&wh);
}

/**
 * Remove the warehouse and its wares from the economy.
 */
void Economy::remove_warehouse(Warehouse& wh) {
	for (size_t i = 0; i < warehouses_.size(); ++i) {
		if (warehouses_[i] == &wh) {
			warehouses_[i] = *warehouses_.rbegin();
			warehouses_.pop_back();

			// Trigger districts recalculation
			for (Flag* flag : flags_) {
				if (flag->get_district_center(type()) == &wh) {
					flag->set_district_center(type(), nullptr);
				}
			}
			start_request_timer();

			return;
		}
	}

	//  This assert was modified, since on loading, warehouses might try to
	//  remove themselves from their own economy, though they weren't added
	//  (since they weren't initialized)
	assert(warehouses_.empty());
}

/**
 * Consider the request, try to fulfill it immediately or queue it for later.
 * Important: This must only be called by the \ref Request class.
 */
void Economy::add_request(Request& req) {
	assert(!has_request(req));

	assert(&owner());

	requests_.push_back(&req);

	// Try to fulfill the request
	start_request_timer();
}

/**
 * \return true if the given Request is registered with the \ref Economy, false
 * otherwise
 */
bool Economy::has_request(Request& req) const {
	return std::find(requests_.begin(), requests_.end(), &req) != requests_.end();
}

/**
 * Remove the request from this economy.
 * Important: This must only be called by the \ref Request class.
 */
void Economy::remove_request(Request& req) {
	RequestList::iterator const it = std::find(requests_.begin(), requests_.end(), &req);

	if (it == requests_.end()) {
		FORMAT_WARNINGS_OFF
		log_warn_time(owner().egbase().get_gametime(), "remove_request(%p) not in list\n", &req);
		FORMAT_WARNINGS_ON
		return;
	}

	*it = *requests_.rbegin();

	requests_.pop_back();
}

/**
 * Add a supply to our list of supplies.
 */
void Economy::add_supply(Supply& supply) {
	supplies_.add_supply(supply);
	start_request_timer();
}

/**
 * Remove a supply from our list of supplies.
 */
void Economy::remove_supply(Supply& supply) {
	supplies_.remove_supply(supply);
}

// Minimal invasive fix of bug 1236538 and issue #3794.
// It does not matter which tribe this soldier has, only that all training levels are 0.
std::unique_ptr<Worker> Economy::soldier_prototype_(nullptr);
// static
Worker& Economy::soldier_prototype(const WorkerDescr* d) {
	if (!soldier_prototype_) {
		if (d == nullptr) {
			throw wexception("soldier_prototype_ not initialized and no SoldierDescr provided");
		}
		assert(d->type() == MapObjectType::SOLDIER);
		soldier_prototype_.reset(&dynamic_cast<Worker&>(d->create_object()));
		assert(soldier_prototype_->descr().type() == MapObjectType::SOLDIER);
	}
	return *soldier_prototype_;
}

bool Economy::needs_ware_or_worker(DescriptionIndex const ware_or_worker_type,
                                   const Flag* flag) const {
	Quantity const target_global = target_quantity(ware_or_worker_type).permanent;
	const bool is_soldier = type_ == wwWORKER && ware_or_worker_type == owner().tribe().soldier();

	if (target_global > 0 && !warehouses_.empty()) {
		// We have a target quantity set.
		Quantity target_district = 0;
		if (flag != nullptr && nr_districts_ > 0) {
			target_district = target_global / nr_districts_;
			if (target_district * nr_districts_ < target_global) {
				++target_district;  // Rounding up is important for wares with small targets
			}
		}

		bool this_district_has_normal = false;
		bool this_district_has_prefer = false;
		bool other_district_has_prefer = false;
		Quantity quantity_global = 0;
		Quantity quantity_district = 0;
		for (const Warehouse* wh : warehouses_) {
			Quantity stock = type_ == wwWARE ? wh->get_wares().stock(ware_or_worker_type) :
                                            wh->get_workers().stock(ware_or_worker_type);
			if (is_soldier) {
				Quantity garrison = wh->get_desired_soldier_count();
				if (garrison >= stock) {
					continue;
				}
				stock -= garrison;
			}

			quantity_global += stock;

			if (flag != nullptr) {
				const StockPolicy policy = type() == wwWARE ?
                                          wh->get_ware_policy(ware_or_worker_type) :
                                          wh->get_worker_policy(ware_or_worker_type);
				if (flag->get_district_center(type()) == wh->base_flag().get_district_center(type())) {
					quantity_district += stock;

					this_district_has_normal |= policy == StockPolicy::kNormal;
					this_district_has_prefer |= policy == StockPolicy::kPrefer;

					if (quantity_district >= target_district && quantity_global >= target_global) {
						// If a district is specified, the ware is needed if the district lacks it
						// even if the global stock is above target.
						// If the global stock is below target we also need it.
						return false;
					}
				} else {
					other_district_has_prefer |= policy == StockPolicy::kPrefer;
				}
			} else if (quantity_global >= target_global) {
				return false;
			}
		}

		if (flag != nullptr && quantity_global >= target_global) {
			// We only need the ware locally but not globally. But...
			if (!this_district_has_normal && !this_district_has_prefer) {
				return false;  // we can't store it, so we don't need it.
			}
			if (other_district_has_prefer && !this_district_has_prefer) {
				return false;  // another district would take it all.
			}
		}

		return true;  // Ware is needed globally or locally.
	}

	// Target quantity is set to 0, we need to check if there is an open request.
	// For soldier requests, do not recruit new rookies if only heroes are needed.
	return std::any_of(
	   requests_.begin(), requests_.end(),
	   [this, is_soldier, ware_or_worker_type](const Request* req) {
		   return req->get_type() == type_ && req->get_index() == ware_or_worker_type &&
		          req->is_open() &&
		          (!is_soldier ||
		           req->get_requirements().check(soldier_prototype(
		              owner().egbase().descriptions().get_worker_descr(ware_or_worker_type))));
	   });
}

bool Economy::has_building(const DescriptionIndex di) const {
	const std::string& name = owner().tribe().get_building_descr(di)->name();
	const auto& stats = owner().get_building_statistics(di);
	return std::any_of(stats.begin(), stats.end(), [this, &name](const auto& bld) {
		const BaseImmovable* i = owner().egbase().map()[bld.pos].get_immovable();
		return !bld.is_constructionsite && i != nullptr && i->descr().name() == name &&
		       dynamic_cast<const Building&>(*i).get_economy(type_) == this;
	});
}

bool Economy::check_building_can_start_working(const ProductionSite& ps,
                                               const bool check_inputqueues) {
	// We are only interested in buildings in this road network
	if (ps.get_economy(type_) != this) {
		return false;
	}
	// Ignore unoccupied buildings
	if (!ps.can_start_working()) {
		return false;
	}

	if (!check_inputqueues || ps.inputqueues().empty()) {
		// If the caller is not interested in the input queues
		// (or if there are none) we are satisfied.
		return true;
	}

	return std::any_of(ps.inputqueues().begin(), ps.inputqueues().end(), [](const InputQueue* q) {
		// At least one input queue needs to be filled.
		return q->get_max_fill() > 0 && q->get_filled() > 0;
	});
}

ProductionSite* Economy::find_closest_occupied_productionsite(const Flag& base,
                                                              const DescriptionIndex di,
                                                              const bool check_inputqueues) {
	const std::string& name = owner().tribe().get_building_descr(di)->name();
	ProductionSite* best = nullptr;
	uint32_t closest_dist = std::numeric_limits<uint32_t>::max();

	for (const auto& bld : owner().get_building_statistics(di)) {
		if (!bld.is_constructionsite) {
			if (BaseImmovable* i = owner().egbase().map()[bld.pos].get_immovable()) {
				if (i->descr().name() == name) {
					upcast(ProductionSite, ps, i);
					assert(ps);
					if (check_building_can_start_working(*ps, check_inputqueues)) {
						const uint32_t dist =
						   owner().egbase().map().calc_distance(base.get_position(), ps->get_position());
						if (dist < closest_dist) {
							closest_dist = dist;
							best = ps;
						}
					}
				}
			}
		}
	}
	return best;
}

/**
 * Add e's flags to this economy.
 *
 * Also transfer all wares and wares request. Try to resolve the new ware
 * requests if possible.
 */
void Economy::merge(Economy& e) {
	assert(e.type() == type_);
	for (const DescriptionIndex& w_index :
	     (type_ == wwWARE ? owner_.tribe().wares() : owner_.tribe().workers())) {
		TargetQuantity other_tq = e.target_quantities_[w_index];
		TargetQuantity& this_tq = target_quantities_[w_index];
		if (this_tq.last_modified < other_tq.last_modified) {
			this_tq = other_tq;
		}
	}

	//  If the options window for e is open, but not the one for this, the user
	//  should still have an options window after the merge.
	if ((e.get_options_window() != nullptr) && (get_options_window() == nullptr)) {
		Notifications::publish(NoteEconomy{e.serial(), serial_, NoteEconomy::Action::kMerged});
	}

	for (std::vector<Flag*>::size_type i = e.get_nrflags() + 1; --i != 0u;) {
		assert(i == e.get_nrflags());

		Flag& flag = *e.flags_[0];

		e.do_remove_flag(flag);  // do not delete other economy yet!
		add_flag(flag);
	}

	// Remember that the other economy may not have been connected before the merge
	split_checks_.insert(split_checks_.end(), e.split_checks_.begin(), e.split_checks_.end());
	owner_.remove_economy(e.serial());
}

/**
 * Split the given set of flags off into a new economy.
 */
void Economy::split(const std::set<OPtr<Flag>>& flags) {
	assert(!flags.empty());

	Economy* e = owner_.create_economy(type_);

	for (const DescriptionIndex& w_index :
	     (type_ == wwWARE ? owner_.tribe().wares() : owner_.tribe().workers())) {
		e->target_quantities_[w_index] = target_quantities_[w_index];
	}

	for (const OPtr<Flag>& temp_flag : flags) {
		Flag& flag = *temp_flag.get(owner().egbase());
		assert(flags_.size() > 1);  // We will not be deleted in remove_flag, right?
		remove_flag(flag);
		e->add_flag(flag);
	}

	e->recalc_districts();

	// As long as rebalance commands are tied to specific flags, we
	// need this, because the flag that rebalance commands for us were
	// tied to might have been moved into the other economy
	start_request_timer();
	e->start_request_timer();
}

/**
 * Make sure the request timer is running.
 * We can skip this for flagless economies (expedition ships don't need economy balancing...).
 */
void Economy::start_request_timer(const Duration& delta) {
	if (!flags_.empty()) {
		if (upcast(Game, game, &owner_.egbase())) {
			Flag* flag = get_arbitrary_flag();
			if (flag->get_economy(type()) == this) {  // Sanity check during economy splits
				game->cmdqueue().enqueue(new CmdCallEconomyBalance(
				   game->get_gametime() + delta, flag, type(), request_timerid_));
			}
		}
	}
}

// make these members, replacing available_supplies_
// (only kept around to avoid always creating and destroying them)
static std::vector<Supply*> possible_imports;
static std::vector<Supply*> possible_supplies;  // std::map<Flag*, Supply*> ?
static std::vector<Widelands::RoutingNode*> possible_flags;  // std::set ?

constexpr uint32_t kTryImportThreshold = 8;  // straight line distance in nodes count

/**
 * Find the supply within search_radius that is best suited to fulfill the given request.
 * \return 0 if no supply is found, the best supply otherwise
 */
Supply*
Economy::find_best_supply(Game& game, const Request& req, int32_t& cost, const uint32_t search_radius) {
	Flag& target_flag = req.target_flag();
	const bool all_in_second_pass = search_radius < std::numeric_limits<uint32_t>::max();
	uint32_t dist_min = search_radius;

	possible_imports.clear();
	possible_supplies.clear();
	possible_flags.clear();

	for (size_t i = 0; i < supplies_.get_nrsupplies(); ++i) {
		Supply& supp = supplies_[i];

		// Just skip if supply does not provide required ware
		if (supp.nr_supplies(game, req) == 0u) {
			continue;
		}

		const SupplyProviders provider = supp.provider_type(&game);

		// We generally ignore disponible wares on ship as it is not possible to reliably
		// calculate route (transportation time)
		if (provider == SupplyProviders::kShip) {
			continue;
		}

		Widelands::Flag& supp_flag = supp.get_position(game)->base_flag();

		// If we are searching for a new supply, then we consider all available local ones within
		// the requesting district for safety, because district assignment considers actual
		// possible routes that our quick distance estimate can not. This way the search radius
		// for imports is limited from the beginning if there are local supplies.
		if (all_in_second_pass || !same_district(target_flag, supp_flag)) {
			// Do not consider imports in the first pass, but save them in case we don't find
			// any local supply.
			// If we are searching for a possible replacement, treat local the same as import,
			// as we already have an upper limit for the search radius.
			possible_imports.emplace_back(&supp);
			continue;
		}

		// Use birds-eye distance for performance, may be inaccurate.
		// We only need to keep the shortest distance, to see whether we should consider
		// nearby supplies from neighbouring districts.
		dist_min = std::min(
		   dist_min, game.map().calc_distance(target_flag.get_position(), supp_flag.get_position()));

		possible_supplies.emplace_back(&supplies_[i]);
		possible_flags.emplace_back(&supp_flag);
	}

	// Try imports if no local supply was found
	if (!possible_imports.empty() && (all_in_second_pass || dist_min >= kTryImportThreshold)) {
		for (Supply* supp : possible_imports) {
			Widelands::Flag& supp_flag = supp->get_position(game)->base_flag();
			const uint32_t dist = game.map().calc_distance(target_flag.get_position(), supp_flag.get_position());

			// Give some allowance as routes may not be straight and flat.
			// dist_min may still be max<uint32_t> if there was no local supply. So this should also
			// take care of always storing the first found supply for new searches.
			if (dist / 2 < dist_min) {
				possible_supplies.emplace_back(supp);
				possible_flags.emplace_back(&supp_flag);
				dist_min = std::min(dist_min, dist);
				// Yes, previous far supplies remain in the list, we only prevent adding new ones.
				// Most of the time we don't have many candidates anyway, so trying to clean up previous
				// ones is not worth the trouble.
			}
		}
	}

	if (possible_flags.empty()) {
		return nullptr;
	}

	Supply* best_supply = nullptr;
	Route best_route;
	Flag* best_supp_flag;

	bool success = false;

	if (possible_flags.size() == 1) {  // faster than messing with a single element vector
		success = router_->find_route(*possible_flags[0], target_flag, &best_route, type_, -1,
		                    *owner().egbase().mutable_map());
	} else {
		success = router_->find_nearest(
	       possible_flags, target_flag, &best_route, type_, *owner().egbase().mutable_map());
	}

	if (!success) {
		log_err_time(
		   game.get_gametime(),
		   "Economy::find_best_supply: %s-Economy %u of player %u: Error, COULD NOT FIND A ROUTE!\n",
		   type_ ? "WORKER" : "WARE", serial_, owner_.player_number());
		return nullptr;
	}

	best_supp_flag = best_route.get_flag_raw(game, 0);

	// Ugly as hell, but I'm not sure how slow storing them in a std::map<Flag*, Supply*> would be
	int i = 0;
	for ( ; possible_flags.at(i) != best_supp_flag ; ++i) {
	}
	best_supply = possible_supplies.at(i);

	cost = best_route.get_totalcost();
	return best_supply;
}

struct RequestSupplyPair {
	TrackPtr<Request> request;
	TrackPtr<Supply> supply;
	uint32_t priority;

	/**
	 * pairid is an explicit tie-breaker for comparison.
	 *
	 * Without it, the pair priority queue would use an implicit, system
	 * dependent tie breaker, which in turn causes desyncs.
	 */
	uint32_t pairid;

	struct Compare {
		bool operator()(const RequestSupplyPair& p1, const RequestSupplyPair& p2) {
			return p1.priority == p2.priority ? p1.pairid < p2.pairid : p1.priority < p2.priority;
		}
	};
};

using RSPairQueue = std::
   priority_queue<RequestSupplyPair, std::vector<RequestSupplyPair>, RequestSupplyPair::Compare>;

struct RSPairStruct {
	RSPairQueue queue;
	uint32_t pairid = 0;
	int32_t nexttimer = 0;

	RSPairStruct() = default;
};

/**
 * Walk all Requests and find potential transfer candidates.
 */
void Economy::process_requests(Game& game, RSPairStruct* supply_pairs) {
	// Algorithm can decide that wares are not to be delivered to constructionsite
	// right now, therefore we need to shcedule next pairing
	for (Request* temp_req : requests_) {
		Request& req = *temp_req;
		if (!req.is_open()) {
			continue;
		}

		// We somehow get desynced request lists that don't trigger desync
		// alerts, so add info to the sync stream here.
		{
			::StreamWrite& ss = game.syncstream();
			ss.unsigned_8(SyncEntry::kProcessRequests);
			ss.unsigned_8(req.get_type());
			ss.unsigned_8(req.get_index());
			ss.unsigned_32(req.target().serial());
		}

		int32_t cost;  // estimated time in milliseconds to fulfill Request
		Supply* const supp = find_best_supply(game, req, cost);

		if (supp == nullptr) {
			continue;
		}

		if (!supp->is_active()) {
			// Calculate the time the building will be forced to idle waiting
			// for the request
			const int32_t idletime =
			   game.get_gametime().get() + 15000 + 2 * cost - req.get_required_time().get();
			// If the building wouldn't have to idle, we wait with the request
			if (idletime < -200) {
				if (supply_pairs->nexttimer < 0 || supply_pairs->nexttimer > -idletime) {
					supply_pairs->nexttimer = -idletime;
				}
				continue;
			}
		}

		const uint32_t priority = req.get_priority(cost);

		RequestSupplyPair rsp;
		rsp.request = &req;
		rsp.supply = supp;
		rsp.priority = priority;
		rsp.pairid = ++supply_pairs->pairid;

		supply_pairs->queue.push(rsp);
	}
}

/**
 * Try to fulfill open requests with available supplies.
 */
void Economy::balance_requestsupply(Game& game) {
	RSPairStruct rsps;
	rsps.nexttimer = -1;

	//  Try to fulfill Requests.
	process_requests(game, &rsps);

	//  Now execute request/supply pairs.

	// Requests with the lowest possible priority are served only if there is nothing else to do
	const bool first_priority_is_0 = !rsps.queue.empty() && rsps.queue.top().priority == 0;

	while (!rsps.queue.empty()) {
		RequestSupplyPair rsp = rsps.queue.top();

		rsps.queue.pop();

		if ((rsp.request == nullptr) || (rsp.supply == nullptr) || !has_request(*rsp.request) ||
		    (rsp.supply->nr_supplies(game, *rsp.request) == 0u) ||
		    (!first_priority_is_0 && rsp.priority == 0)) {
			rsps.nexttimer = 200;
			continue;
		}

		rsp.request->start_transfer(game, *rsp.supply);
		rsp.request->set_last_request_time(game.get_gametime());

		//  for multiple wares
		if ((rsp.request != nullptr) && has_request(*rsp.request)) {
			rsps.nexttimer = 200;
		}
	}

	if (rsps.nexttimer > 0) {  //  restart the timer, if necessary
		start_request_timer(Duration(rsps.nexttimer));
	}
}

/**
 * Check whether there is a supply for the given request. If the request is a
 * worker request without supply, attempt to create a new worker in a warehouse.
 */
void Economy::create_requested_worker(Game& game, DescriptionIndex index) {
	assert(type_ == wwWORKER);

	bool soldier_level_check;
	const TribeDescr& tribe = owner().tribe();
	const WorkerDescr& w_desc = *tribe.get_worker_descr(index);
	// Request mapped to demand
	std::map<Request*, uint32_t> open_requests;
	uint32_t total_demand = 0;

	// Make a dummy soldier, which should never be assigned to any economy
	// Minimal invasive fix of bug 1236538: never create a rookie for a request
	// that required a hero.
	if (upcast(const SoldierDescr, s_desc, &w_desc)) {
		soldier_prototype(s_desc);  // init prototype
		soldier_level_check = true;
	} else {
		soldier_level_check = false;
	}

	for (Request* temp_req : requests_) {
		const Request& req = *temp_req;

		if (!req.is_open() || req.get_type() != wwWORKER || req.get_index() != index) {
			continue;
		}

		// need to check for each request separately, because esp. soldier
		// requests have different specific requirements
		if (supplies_.have_supplies(game, req)) {
			continue;
		}

		// Requests for heroes should not trigger the creation of more rookies
		if (soldier_level_check) {
			if (!(req.get_requirements().check(soldier_prototype()))) {
				continue;
			}
		}

		uint32_t current_demand = req.get_open_count();
		if (current_demand > 0) {
			open_requests.emplace(temp_req, current_demand);
			total_demand += current_demand;
		}
	}

	if (total_demand == 0) {
		assert(open_requests.empty());
		return;
	}
	assert(!open_requests.empty());

	// We have worker demands that are not fulfilled by supplies.
	// Find warehouses where we can create the required workers,
	// and collect stats about existing build prerequisites.
	// Since the wares may be in places belonging to a different worker economy,
	// we will request their ware economies to bring them into warehouses belonging to this worker
	// economy.
	const WorkerDescr::Buildcost& cost = w_desc.buildcost();
	Quantity total_planned = 0;
	std::map<Economy*, std::map<DescriptionIndex, Quantity>> available_wares;

	for (Warehouse* wh : warehouses_) {
		uint32_t planned = wh->get_planned_workers(game, index);
		total_planned += planned;

		while (wh->can_create_worker(game, index)) {
			wh->create_worker(game, index);
			--open_requests.begin()->second;
			--total_demand;
			if (open_requests.begin()->second == 0u) {
				open_requests.erase(open_requests.begin());
			}
			if (total_demand == 0) {
				assert(open_requests.empty());
				return;
			}
		}

		Economy* eco = wh->get_economy(wwWARE);
		for (const auto& pair : cost) {
			DescriptionIndex di = tribe.ware_index(pair.first);
			if (tribe.has_ware(di)) {
				available_wares[eco][di] += eco->get_wares_or_workers().stock(di);
			}
		}
	}

	// Couldn't create enough workers now. Adjust the warehouses' plans to bring the wares together.
	for (const auto& pair : available_wares) {
		uint32_t min_workers_createable = std::numeric_limits<Quantity>::max();
		bool plan_at_least_one = false;
		for (const auto& costpair : cost) {
			DescriptionIndex di = tribe.ware_index(costpair.first);
			if (tribe.has_ware(di)) {
				min_workers_createable =
				   std::min(min_workers_createable, pair.second.at(di) / costpair.second);
				plan_at_least_one |= pair.first->target_quantity(di).permanent == 0;
			} else {
				di = tribe.safe_worker_index(costpair.first);
				assert(tribe.has_worker(di));
				min_workers_createable = std::max(min_workers_createable, wares_or_workers_.stock(di));
				// TODO(Nordfriese): As long as worker buildcosts contain only wares and carriers, this
				// is fine. Revisit this function if we ever have a worker whose buildcost contains a
				// worker with a buildcost.
			}
		}
		for (Warehouse* wh : warehouses_) {
			if (wh->get_economy(wwWARE) == pair.first) {
				const uint32_t planned = wh->get_planned_workers(game, index);
				assert(total_planned >= planned);
				uint32_t nr_to_plan = planned;
				if (total_planned > total_demand) {
					// Cancel some excess plans
					nr_to_plan -= std::min(nr_to_plan, total_planned - total_demand);
				} else if (total_planned < total_demand) {
					// Check how many we can plan
					if (min_workers_createable > 0) {
						uint32_t newly_planning =
						   std::min(min_workers_createable, total_demand - total_planned);
						min_workers_createable -= newly_planning;
						nr_to_plan += newly_planning;
					} else if (plan_at_least_one) {
						// Plan at least one worker somewhere if a target quantity is 0 to trigger tool
						// production
						nr_to_plan = std::max(nr_to_plan, 1u);
						plan_at_least_one = false;
					}
				}
				wh->plan_workers(game, index, nr_to_plan);
				total_planned = total_planned + nr_to_plan - planned;
				if (total_planned == total_demand) {
					return;
				}
			}
		}
	}
}

/**
 * Walk all Requests and find requests of workers than aren't supplied. Then
 * try to create the worker at warehouses.
 */
void Economy::create_requested_workers(Game& game) {
	if (type_ != wwWORKER || warehouses().empty()) {
		return;
	}

	for (const DescriptionIndex& worker_index : owner().tribe().workers()) {
		if (owner().is_worker_type_allowed(worker_index) &&
		    owner().tribe().get_worker_descr(worker_index)->is_buildable()) {
			create_requested_worker(game, worker_index);
		}
	}
}

/**
 * Helper function for \ref handle_active_supplies
 */
static bool accept_warehouse_if_policy(const Warehouse& wh,
                                       WareWorker type,
                                       DescriptionIndex ware,
                                       StockPolicy policy) {
	return wh.get_stock_policy(type, ware) == policy;
}

/**
 * Send all active supplies (wares that are outside on the road network without
 * being sent to a specific request) to a warehouse.
 */
void Economy::handle_active_supplies(Game& game) {
	if (warehouses().empty()) {
		return;
	}

	using Assignments = std::vector<std::pair<Supply*, Warehouse*>>;
	Assignments assignments;

	for (uint32_t idx = 0; idx < supplies_.get_nrsupplies(); ++idx) {
		Supply& supply = supplies_[idx];
		if (supply.has_storage()) {
			continue;
		}

		WareWorker wwtype;
		DescriptionIndex ware;
		supply.get_ware_type(wwtype, ware);
		assert(wwtype == type_);

		bool haveprefer = false;
		bool havenormal = false;

		// One of preferred warehouses (if any) with lowest stock of ware/worker
		Warehouse* preferred_wh = nullptr;
		// Stock of particular ware in preferred warehouse
		uint32_t preferred_wh_stock = std::numeric_limits<uint32_t>::max();

		for (Warehouse* wh : warehouses_) {
			StockPolicy policy = wh->get_stock_policy(type_, ware);
			if (policy == StockPolicy::kPrefer) {
				haveprefer = true;

				// Getting count of worker/ware
				uint32_t current_stock;
				current_stock =
				   type_ == wwWARE ? wh->get_wares().stock(ware) : wh->get_workers().stock(ware);
				// Stocks lower then in previous one?
				if (current_stock < preferred_wh_stock) {
					preferred_wh = wh;
					preferred_wh_stock = current_stock;
				}
			} else if (policy == StockPolicy::kNormal) {
				havenormal = true;
			}
		}
		if (!havenormal && !haveprefer && type_ == wwWARE) {
			continue;
		}

		// We either have one preferred warehouse picked up or walk on roads to find nearest one
		Warehouse* wh = nullptr;
		if (preferred_wh != nullptr) {
			wh = preferred_wh;
		} else {
			wh = find_closest_warehouse(
			   supply.get_position(game)->base_flag(), nullptr, 0,
			   (!havenormal) ? WarehouseAcceptFn() : [this, ware](const Warehouse& w) {
				   return accept_warehouse_if_policy(w, type_, ware, StockPolicy::kNormal);
			   });
		}
		if (wh == nullptr) {
			log_warn_time(game.get_gametime(), "Warning: Economy::handle_active_supplies "
			                                   "didn't find warehouse\n");
			return;
		}

		assignments.emplace_back(&supply, wh);
	}

	// Actually start with the transfers in a separate second phase,
	// to avoid potential future problems caused by the supplies_ changing
	// under us in some way.
	::StreamWrite& ss = game.syncstream();
	ss.unsigned_8(SyncEntry::kHandleActiveSupplies);
	ss.unsigned_32(assignments.size());

	for (const auto& temp_assignment : assignments) {
		ss.unsigned_32(temp_assignment.first->get_position(game)->serial());
		ss.unsigned_32(temp_assignment.second->serial());

		temp_assignment.first->send_to_storage(game, temp_assignment.second);
	}
}

bool Economy::same_district(const PlayerImmovable& p1, const PlayerImmovable& p2) const {
	const Warehouse* wh = p1.base_flag().get_district_center(type());
	return wh != nullptr && wh == p2.base_flag().get_district_center(type());
}

// Note: Do not call this function when pending road network splits and merges may exist!
void Economy::recalc_districts() {
	if (flags_.empty()) {
		nr_districts_ = 0;
		return;
	}

	if (warehouses_.empty()) {
		// Special case for economy without warehouses: Obviously there are no districts.
		nr_districts_ = 0;
		for (Flag* flag : flags_) {
			flag->set_district_center(type(), nullptr);
		}
		return;
	}

	// Assign each flag to the closest warehouse.
	RouteAStar<AStarZeroEstimator> astar(*router_, type_, AStarZeroEstimator());
	for (Warehouse* wh : warehouses_) {
		wh->base_flag().set_district_center(type(), wh);
		astar.push(wh->base_flag());
	}
	{
		RoutingNode* current;
		while ((current = astar.step()) != nullptr) {
			current->base_flag().set_district_center(
			   type(), astar.route_start(*current).base_flag().get_district_center(type()));
		}
	}

	nr_districts_ = warehouses_.size();
}

// Number of remaining route steps above which we consider replacement
// TODO(tothxa): Should be dynamic and use cost instead. We should store the route cost
//               for each flag in recalc_districts(), then we could search for replacement
//               for transfers with remaining cost > cost from local warehouse.
//               (with a mandatory minimum)
constexpr uint32_t kTryOtherSupplyThreshold = 12;

// Don't replace existing transfer if cost saving is less than these:
// Minimum absolute saving (affects shorter route replacements):
constexpr int kSupplyReplacementMinSaving = kTryOtherSupplyThreshold * 1800 / 2;
// Minimum ratio of saving to existing is 1/4 (affects longer route replacements):
constexpr int kSupplyReplacementRatio = 4;

/** Cancel cross-district imports when suitable wares become available locally. */
void Economy::check_imports(Game& game) {
	for (Request* request : requests_) {
		if (request->get_num_transfers() == 0) {
			continue;
		}

		Flag& target_flag = request->target().base_flag();
		Warehouse* target_district = target_flag.get_district_center(type());
		if (target_district == nullptr) {
			continue;
		}

		for (size_t i = 0; i < request->get_num_transfers(); ++i) {
			MapObject* location;
			if (request->get_transfers()[i]->get_ware() != nullptr) {
				location = request->get_transfers()[i]->get_ware()->get_location(game);
			} else {
				location = request->get_transfers()[i]->get_worker()->get_location(game);
			}

			upcast(PlayerImmovable, imm, location);
			if (imm == nullptr) {
				continue;
			}

			Flag& supply_flag = imm->base_flag();
			Warehouse* supply_district = supply_flag.get_district_center(type());
			if (supply_district == nullptr) {
				continue;
			}

			const uint32_t supply_distance = request->get_transfers()[i]->get_steps_left();
			if (supply_distance <= kTryOtherSupplyThreshold) {
				continue;
			}

			if (supply_distance == std::numeric_limits<uint32_t>::max()) {
				// get_steps_left() has uint wraparound if the route is empty...
				// doesn't look intentional, and it does happen quite often, but didn't
				// seem to cause problems... maybe it's best to leave such empty routes
				// alone and leave it to normal handling to pick them up?
				continue;
			}

			// The transfer we are looking at is too far. Check for better ones.
			// supply_distance is actual road distance, while search radius is straight line distance,
			// and we only care about significant savings.
			int cost;
			Supply* near_supply = find_best_supply(game, *request, cost, supply_distance / 2);

			if (near_supply == nullptr) {
				continue;
			}

			// Prefer old one, only replace if cost saving is significant
			const int old_cost = request->get_transfers()[i]->get_cost_left();
			int saving = old_cost - cost;
			if (saving < kSupplyReplacementMinSaving ||
				 saving < old_cost / kSupplyReplacementRatio) {
				continue;
			}

			request->cancel_transfer(i);
			request->start_transfer(game, *near_supply);
			break;  // Exchange at most one import per request per cycle.
		}
	}
}

/**
 * Balance Requests and Supplies by collecting and weighing pairs, and
 * starting transfers for them.
 */
void Economy::balance(uint32_t const timerid) {
	if (request_timerid_ != timerid) {
		return;
	}
	++request_timerid_;

	Game& game = dynamic_cast<Game&>(owner().egbase());

	check_splits();

	recalc_districts();

	create_requested_workers(game);

	balance_requestsupply(game);

	handle_active_supplies(game);

	check_imports(game);

}
}  // namespace Widelands
