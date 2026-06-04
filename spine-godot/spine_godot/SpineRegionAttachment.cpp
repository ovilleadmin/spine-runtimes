/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated April 5, 2025. Replaces all prior versions.
 *
 * (license header identical to other files in this folder; abridged)
 *****************************************************************************/

#include "SpineRegionAttachment.h"

void SpineRegionAttachment::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_region", "region"), &SpineRegionAttachment::set_region);
	ClassDB::bind_method(D_METHOD("get_spine_texture_region"), &SpineRegionAttachment::get_spine_texture_region);
	ClassDB::bind_method(D_METHOD("update_region"), &SpineRegionAttachment::update_region);

	ClassDB::bind_method(D_METHOD("get_x"), &SpineRegionAttachment::get_x);
	ClassDB::bind_method(D_METHOD("set_x", "x"), &SpineRegionAttachment::set_x);
	ClassDB::bind_method(D_METHOD("get_y"), &SpineRegionAttachment::get_y);
	ClassDB::bind_method(D_METHOD("set_y", "y"), &SpineRegionAttachment::set_y);
	ClassDB::bind_method(D_METHOD("get_width"), &SpineRegionAttachment::get_width);
	ClassDB::bind_method(D_METHOD("set_width", "w"), &SpineRegionAttachment::set_width);
	ClassDB::bind_method(D_METHOD("get_height"), &SpineRegionAttachment::get_height);
	ClassDB::bind_method(D_METHOD("set_height", "h"), &SpineRegionAttachment::set_height);
	ClassDB::bind_method(D_METHOD("get_rotation"), &SpineRegionAttachment::get_rotation);
	ClassDB::bind_method(D_METHOD("set_rotation", "r"), &SpineRegionAttachment::set_rotation);
	ClassDB::bind_method(D_METHOD("get_scale_x"), &SpineRegionAttachment::get_scale_x);
	ClassDB::bind_method(D_METHOD("set_scale_x", "s"), &SpineRegionAttachment::set_scale_x);
	ClassDB::bind_method(D_METHOD("get_scale_y"), &SpineRegionAttachment::get_scale_y);
	ClassDB::bind_method(D_METHOD("set_scale_y", "s"), &SpineRegionAttachment::set_scale_y);

	ClassDB::bind_method(D_METHOD("get_color"), &SpineRegionAttachment::get_color);
	ClassDB::bind_method(D_METHOD("set_color", "c"), &SpineRegionAttachment::set_color);

	ClassDB::bind_method(D_METHOD("has_sequence"), &SpineRegionAttachment::has_sequence);
}

void SpineRegionAttachment::set_region(Ref<SpineTextureRegion> region) {
	SPINE_CHECK(_region_attachment(), )
	if (!region.is_valid() || !region->get_spine_object()) {
		ERR_PRINT("SpineRegionAttachment::set_region: region is invalid");
		return;
	}
	owned_region = region; // keep alive as long as this attachment wrapper is alive
	_region_attachment()->setRegion(region->get_spine_object());
	_region_attachment()->updateRegion();
}

Ref<SpineTextureRegion> SpineRegionAttachment::get_spine_texture_region() {
	return owned_region;
}

void SpineRegionAttachment::update_region() {
	SPINE_CHECK(_region_attachment(), )
	_region_attachment()->updateRegion();
}

float SpineRegionAttachment::get_x()        { SPINE_CHECK(_region_attachment(), 0) return _region_attachment()->getX(); }
void  SpineRegionAttachment::set_x(float x) { SPINE_CHECK(_region_attachment(), )  _region_attachment()->setX(x); }
float SpineRegionAttachment::get_y()        { SPINE_CHECK(_region_attachment(), 0) return _region_attachment()->getY(); }
void  SpineRegionAttachment::set_y(float y) { SPINE_CHECK(_region_attachment(), )  _region_attachment()->setY(y); }
float SpineRegionAttachment::get_width()        { SPINE_CHECK(_region_attachment(), 0) return _region_attachment()->getWidth(); }
void  SpineRegionAttachment::set_width(float w) { SPINE_CHECK(_region_attachment(), )  _region_attachment()->setWidth(w); }
float SpineRegionAttachment::get_height()        { SPINE_CHECK(_region_attachment(), 0) return _region_attachment()->getHeight(); }
void  SpineRegionAttachment::set_height(float h) { SPINE_CHECK(_region_attachment(), )  _region_attachment()->setHeight(h); }
float SpineRegionAttachment::get_rotation()        { SPINE_CHECK(_region_attachment(), 0) return _region_attachment()->getRotation(); }
void  SpineRegionAttachment::set_rotation(float r) { SPINE_CHECK(_region_attachment(), )  _region_attachment()->setRotation(r); }
float SpineRegionAttachment::get_scale_x()        { SPINE_CHECK(_region_attachment(), 1) return _region_attachment()->getScaleX(); }
void  SpineRegionAttachment::set_scale_x(float s) { SPINE_CHECK(_region_attachment(), )  _region_attachment()->setScaleX(s); }
float SpineRegionAttachment::get_scale_y()        { SPINE_CHECK(_region_attachment(), 1) return _region_attachment()->getScaleY(); }
void  SpineRegionAttachment::set_scale_y(float s) { SPINE_CHECK(_region_attachment(), )  _region_attachment()->setScaleY(s); }

Color SpineRegionAttachment::get_color() {
	SPINE_CHECK(_region_attachment(), Color())
	auto &c = _region_attachment()->getColor();
	return Color(c.r, c.g, c.b, c.a);
}

void SpineRegionAttachment::set_color(Color c) {
	SPINE_CHECK(_region_attachment(), )
	_region_attachment()->getColor().set(c.r, c.g, c.b, c.a);
}

bool SpineRegionAttachment::has_sequence() {
	SPINE_CHECK(_region_attachment(), false)
	// spine-cpp 4.3: getSequence() returns a Sequence& (never null — readSequence
	// always creates at least a default 1-region sequence). A "real" animated
	// sequence has more than one region.
	return _region_attachment()->getSequence().getRegions().size() > 1;
}
