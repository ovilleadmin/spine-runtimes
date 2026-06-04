/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated April 5, 2025. Replaces all prior versions.
 *
 * Copyright (c) 2013-2025, Esoteric Software LLC
 *
 * (license header identical to other files in this folder; abridged)
 *****************************************************************************/

#include "SpineTextureRegion.h"

#ifdef SPINE_GODOT_EXTENSION
#include <godot_cpp/classes/canvas_texture.hpp>
#else
#include "scene/resources/canvas_texture.h"
#endif

void SpineTextureRegion::_bind_methods() {
	ClassDB::bind_static_method("SpineTextureRegion", D_METHOD("from_texture", "texture"), &SpineTextureRegion::from_texture);
	ClassDB::bind_method(D_METHOD("set_uv", "u", "v", "u2", "v2"), &SpineTextureRegion::set_uv);
	ClassDB::bind_method(D_METHOD("get_u"), &SpineTextureRegion::get_u);
	ClassDB::bind_method(D_METHOD("get_v"), &SpineTextureRegion::get_v);
	ClassDB::bind_method(D_METHOD("get_u2"), &SpineTextureRegion::get_u2);
	ClassDB::bind_method(D_METHOD("get_v2"), &SpineTextureRegion::get_v2);
	ClassDB::bind_method(D_METHOD("get_texture"), &SpineTextureRegion::get_texture);
}

SpineTextureRegion::SpineTextureRegion() : atlas_region(nullptr), atlas_page(nullptr), renderer_object(nullptr) {
}

SpineTextureRegion::~SpineTextureRegion() {
	// SpineRendererObject is a struct with Ref<Texture> members that get
	// cleanly released when the object is deleted via memdelete.
	if (renderer_object) {
		memdelete(renderer_object);
		renderer_object = nullptr;
	}
	// atlas_page.texture is the void* pointing at renderer_object (already
	// freed above). We null it before delete so spine-cpp's AtlasPage dtor
	// doesn't try to touch it.
	if (atlas_page) {
		atlas_page->texture = nullptr;
		delete atlas_page;
		atlas_page = nullptr;
	}
	if (atlas_region) {
		delete atlas_region;
		atlas_region = nullptr;
	}
}

Ref<SpineTextureRegion> SpineTextureRegion::from_texture(Ref<Texture2D> texture) {
	if (!texture.is_valid()) {
		ERR_PRINT("SpineTextureRegion::from_texture: texture is invalid");
		return Ref<SpineTextureRegion>();
	}

	int tex_width = texture->get_width();
	int tex_height = texture->get_height();
	if (tex_width <= 0 || tex_height <= 0) {
		ERR_PRINT("SpineTextureRegion::from_texture: texture has zero size");
		return Ref<SpineTextureRegion>();
	}

	Ref<SpineTextureRegion> out;
	INSTANTIATE(out);

	// Wrap the Texture2D in a CanvasTexture so spine-godot's render loop can
	// retrieve a RID compatible with RenderingServer::canvas_item_add_mesh().
	Ref<CanvasTexture> canvas_tex;
	INSTANTIATE(canvas_tex);
	canvas_tex->set_diffuse_texture(texture);

	out->source_texture = texture;
	out->renderer_object = memnew(SpineRendererObject);
	out->renderer_object->texture = texture;
	out->renderer_object->normal_map = Ref<Texture>(nullptr);
	out->renderer_object->specular_map = Ref<Texture>(nullptr);
#if VERSION_MAJOR > 3
	out->renderer_object->canvas_texture = canvas_tex;
#endif

	// Allocate a minimal AtlasPage that owns our SpineRendererObject via its
	// texture void*. We don't attach it to any parent atlas.
	out->atlas_page = new spine::AtlasPage(spine::String("spine-godot-runtime-page"));
	out->atlas_page->width = tex_width;
	out->atlas_page->height = tex_height;
	out->atlas_page->texture = (void *) out->renderer_object;

	// AtlasRegion extends TextureRegion; spine-godot's render loop casts the
	// result of attachment->getRegion() to AtlasRegion*, so we must provide
	// one even though most of the atlas-specific fields are unused here.
	out->atlas_region = new spine::AtlasRegion();
	out->atlas_region->page = out->atlas_page;
	out->atlas_region->name = spine::String("spine-godot-runtime-region");
	out->atlas_region->index = -1;
	out->atlas_region->u = 0.0f;
	out->atlas_region->v = 0.0f;
	out->atlas_region->u2 = 1.0f;
	out->atlas_region->v2 = 1.0f;
	out->atlas_region->width = tex_width;
	out->atlas_region->height = tex_height;
	out->atlas_region->originalWidth = tex_width;
	out->atlas_region->originalHeight = tex_height;
	out->atlas_region->offsetX = 0;
	out->atlas_region->offsetY = 0;
	out->atlas_region->degrees = 0;

	return out;
}

void SpineTextureRegion::set_uv(float u, float v, float u2, float v2) {
	if (!atlas_region) return;
	atlas_region->u = u;
	atlas_region->v = v;
	atlas_region->u2 = u2;
	atlas_region->v2 = v2;
}

float SpineTextureRegion::get_u()  { return atlas_region ? atlas_region->u  : 0.0f; }
float SpineTextureRegion::get_v()  { return atlas_region ? atlas_region->v  : 0.0f; }
float SpineTextureRegion::get_u2() { return atlas_region ? atlas_region->u2 : 1.0f; }
float SpineTextureRegion::get_v2() { return atlas_region ? atlas_region->v2 : 1.0f; }

Ref<Texture2D> SpineTextureRegion::get_texture() {
	return source_texture;
}
