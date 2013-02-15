/*
 * Copyright (C) 2002-2004, 2006-2013 by the Widelands Development Team
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <vector>
#include <map>

#include <SDL.h>
#include <boost/scoped_ptr.hpp>
#include <png.h>

#include "image_cache.h"
#include "rect.h"

#define MAX_RECTS 20

namespace UI {struct ProgressWindow;}

class Animation;  // // NOCOM(#sirver): should not be here
class AnimationManager;
class ImageLoaderImpl;
class RenderTarget;
class Screen;
class Surface;
class SurfaceCache;
struct Road_Textures;
struct SDL_Surface;
struct StreamWrite;
struct Texture;

/// Stores the capabilities of opengl
struct GLCaps
{
	/// The OpenGL major version
	int major_version;
	/// The OpenGL minor version
	int minor_version;
	/// The maximum texture size
	int tex_max_size;
	/// If true sizes of texture must be a power of two
	bool tex_power_of_two;
	/// How many bits the stencil buffer support
	int stencil_buffer_bits;
	/// How many Aux Buffers the opengl context support
	int aux_buffers;
	/// Whether the BlendEquation support is available
	bool blendequation;
	/// Maximum number of textures that can be combined
	int max_tex_combined;
	/// Whether multitexturing is supported
	bool multitexture;
};

/**
 * A structure to store the capabilities of the current rendere. This is set
 * during init() and can be retrieved by g_gr->get_caps()
 */
struct GraphicCaps
{
	/// The capabilities of the opengl hardware and drive
	GLCaps gl;
};

/**
 * This class is a kind of Swiss Army knife for your graphics need. It
 * initializes the graphic system and provides access to resolutions. It has an
 * Image and a Surface cache and holds the data of all animations and the road
 * textures. It also offers functionality to save a screenshot.
 */
class Graphic {
public:
	Graphic
		(int32_t w, int32_t h, int32_t bpp,
		 bool fullscreen, bool opengl);
	~Graphic();

	int32_t get_xres() const;
	int32_t get_yres() const;
	RenderTarget * get_render_target();
	void toggle_fullscreen();
	void update_fullscreen();
	void update_rectangle(int32_t x, int32_t y, int32_t w, int32_t h);
	void update_rectangle(const Rect& rect) {
		update_rectangle (rect.x, rect.y, rect.w, rect.h);
	}
	bool need_update() const;
	void refresh(bool force = true);

	SurfaceCache& surfaces() const {return *surface_cache_.get();}
	ImageCache& images() const {return *image_cache_.get();}
	AnimationManager& animations() const {return *animation_manager_.get();}

	void save_png(const Image*, StreamWrite*) const;

	uint32_t get_maptexture(const std::string& fnametempl, uint32_t frametime);
	void animate_maptextures(uint32_t time);
	void reset_texture_animation_reminder();

	void ensure_animation_loaded(uint32_t anim);
	size_t nr_frames(uint32_t anim = 0);
	uint32_t get_animation_frametime(uint32_t anim) const;
	void get_animation_size (uint32_t anim, uint32_t time, uint32_t & w, uint32_t & h);

	void screenshot(const std::string& fname) const;
	Texture * get_maptexture_data(uint32_t id);
	Animation * get_animation(uint32_t);

	void set_world(std::string);
	Surface& get_road_texture(int32_t roadtex);

	const GraphicCaps& caps() const throw () {return m_caps;}

private:
	void save_png_(Surface & surf, StreamWrite*) const;

protected:
	// Static helper function for png writing
	static void m_png_write_function
		(png_structp png_ptr,
		 png_bytep data,
		 png_size_t length);
	static void m_png_flush_function (png_structp png_ptr);

	/// This is the main screen Surface.
	/// A RenderTarget for this can be retrieved with get_render_target()
	boost::scoped_ptr<Surface> screen_;
	/// This saves a copy of the screen SDL_Surface. This is needed for
	/// opengl rendering as the SurfaceOpenGL does not use it. It allows
	/// manipulation the screen context.
	SDL_Surface * m_sdl_screen;
	/// A RenderTarget for screen_. This is initialized during init()
	RenderTarget * m_rendertarget;
	/// keeps track which screen regions needs to be redrawn during the next
	/// update(). Only used for SDL rendering.
	SDL_Rect m_update_rects[MAX_RECTS];
	/// saves how many screen regions need updating. @see m_update_rects
	int32_t m_nr_update_rects;
	/// This marks the komplete screen for updating.
	bool m_update_fullscreen;
	/// stores which features the current renderer has
	GraphicCaps m_caps;

	/// The class that gets images from disk.
	boost::scoped_ptr<ImageLoaderImpl> image_loader_;
	/// Volatile cache of Hardware dependant surfaces.
	boost::scoped_ptr<SurfaceCache> surface_cache_;
	/// Non-volatile cache of hardware independent images. The use the
	/// surface_cache_ to cache their pixel data.
	boost::scoped_ptr<ImageCache> image_cache_;
	/// This holds all animations.
	boost::scoped_ptr<AnimationManager> animation_manager_;

	// The texture needed to draw roads.
	boost::scoped_ptr<Surface> pic_road_normal_;
	boost::scoped_ptr<Surface> pic_road_busy_;

	std::vector<Texture *> m_maptextures;
	std::vector<Animation *> m_animations;
};

extern Graphic * g_gr;
extern bool g_opengl;

#endif
