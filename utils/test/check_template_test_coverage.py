#!/usr/bin/env python3
# encoding: utf-8

import configparser
from glob import glob
import os.path
import re
import sys

# This script checks that all starting conditions of all tribes and all
# win conditions are used in the regression test suite.

print('Checking starting and win conditions test coverage')

basedir = os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir, os.path.pardir))

datadir = os.path.join(basedir, 'data')
if not os.path.isdir(datadir):
    print('ERROR: Data directory ' + datadir + ' not found.')
    sys.exit(1)

# Collect win conditions
def collect_win_conditions():
    wcdir = os.path.join(datadir, 'scripting', 'win_conditions')
    wc_init = os.path.join(wcdir, 'init.lua')
    if not os.path.isfile(wc_init):
        print('ERROR: Win conditions config file not found at ' + wc_init)
        sys.exit(1)

    with open(wc_init, 'r', encoding='utf-8') as fh:
        wc_regexp = re.compile('("[a-z_]+\\.lua")')
        win_conditions = {match.group(1): False
                          for match in [wc_regexp.search(line)
                          for line in fh.read().splitlines()] if match}
    if len(win_conditions) < 1:
        print('ERROR: No win conditions found.')
        sys.exit(1)

    print()
    print('Win conditions:')
    for wc in win_conditions:
        print('   ' + wc)
    return win_conditions
win_conditions = collect_win_conditions()

# Collect start conditions for all tribes
def collect_start_conditions():
    tribesdir = os.path.join(datadir, 'tribes', 'initialization')
    tribes = sorted([td for td in glob(os.path.join(tribesdir, '*'))
                    if os.path.isdir(os.path.join(td, 'starting_conditions'))])
    if len(tribes) < 1:
        print('ERROR: No tribe directories found.')
        sys.exit(1)

    start_conditions = {'"' + os.path.basename(td) + '"':
                        {'"' + os.path.basename(sc) + '"': False
                         for sc in sorted(glob(os.path.join(td, 'starting_conditions', '*')))}
                        for td in tribes}

    if len(start_conditions) < 1:
        print('ERROR: No start conditions found.')
        sys.exit(1)

    print()
    print('Start conditions:')
    for tr in start_conditions:
        print('   ' + tr + ':')
        for sc in start_conditions[tr]:
            print('      ' + sc)
    return start_conditions
start_conditions = collect_start_conditions()

# Collect tests
def collect_tests():
    testdir = os.path.join(basedir, 'test', 'templates')
    tests = sorted(glob(os.path.join(testdir, '*.wgt')))
    if len(tests) < 1:
        print('ERROR: No tests found in directory ' + testdir)
        sys.exit(1)

    unknown_wc = []
    unknown_tribe = []
    unknown_sc = dict()
    no_script = []

    # Collect uses
    for test in tests:
        if not os.path.isfile(test[:-3] + 'lua'):
            no_script.append(test)
        cfg = configparser.ConfigParser()
        cfg.read(test)

        # This one is a bare minimum template, leaves all to defaults, including tribes, and start and
        # win conditions.
        if test == os.path.join(testdir, 'test_alldefaults.wgt'):
            continue

        for s in cfg.sections():
            section = cfg[s]

            # Test's win condition
            if s == 'global':
                if not section['win_condition'] in win_conditions:
                    unknown_wc.append(section['win_condition'])
                else:
                    win_conditions[section['win_condition']] = True

            # Each player's start condition
            if s.startswith('player_') and section['closed'] != '"true"' and \
               section['tribe'] != '':
                if not section['tribe'] in start_conditions:
                    unknown_tribe.append(section['tribe'])
                else:
                    if not section['init'] in start_conditions[section['tribe']]:
                        if not section['tribe'] in unknown_sc:
                            unknown_sc[section['tribe']] = []
                        unknown_sc[section['tribe']].append(section['init'])
                    else:
                        start_conditions[section['tribe']][section['init']] = True
    return {
        'wc': unknown_wc,
        'tribe': unknown_tribe,
        'sc': unknown_sc,
        'no_script': no_script,
    }

# Check unused
def check_unused_wc():
    return [wc for wc in win_conditions if not win_conditions[wc]]

def check_unused_sc():
    unused_sc = dict()
    for tribe in start_conditions:
        unused = [sc for sc in start_conditions[tribe] if not start_conditions[tribe][sc]]
        if len(unused) > 0:
            unused_sc[tribe] = unused
    return unused_sc

# Check all tribes in all_tribes.wmf/
def check_missing_in_alltribes():
    missing_in_alltribes = []
    for tribe in start_conditions:
        tribe = tribe.strip('"')
        path = f'test/maps/all_tribes.wmf/scripting/test_{ tribe }.lua'
        if not os.path.isfile(path):
            missing_in_alltribes.append(tribe)
    return missing_in_alltribes


def run_checks():
    failures = 0

    def report_if(check_result, msg):
        nonlocal failures
        if len(check_result) > 0:
            print()
            print(msg)
            print(check_result)
            failures += 1

    unknown = collect_tests()

    # Report
    report_if(unknown['tribe'], 'ERROR: Unknown tribes found in the tests:')
    report_if(unknown['sc'], 'ERROR: Unknown start conditions found in the tests:')
    report_if(unknown['wc'], 'ERROR: Unknown win conditions found in the tests:')

    report_if(check_unused_sc(), 'ERROR: Start conditions not covered by tests:')
    report_if(check_unused_wc(), 'ERROR: Win conditions not covered by tests:')

    report_if(unknown['no_script'], 'ERROR: No scripts provided for tests:')

    report_if(check_missing_in_alltribes(),
              'ERROR: Test for tribes missing in test/maps/all_tribes.wmf/scripting/:')

    if failures == 0:
        print('Done, all starting and win conditions are covered by tests.')

    return failures

if __name__ == '__main__':
    if run_checks() > 0:
        sys.exit(1)
