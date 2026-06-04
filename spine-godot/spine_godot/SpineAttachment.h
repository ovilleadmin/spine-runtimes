/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated April 5, 2025. Replaces all prior versions.
 *
 * (license header identical to other files in this folder; abridged)
 *****************************************************************************/

#pragma once

#include "SpineCommon.h"
#include "spine/Attachment.h"
#include <spine/spine.h>

class SpineSkeletonDataResource;

class SpineAttachment : public SpineSkeletonDataResourceOwnedObject<spine::Attachment> {
	GDCLASS(SpineAttachment, SpineObjectWrapper)

protected:
	static void _bind_methods();

public:
	~SpineAttachment() override;

	String get_attachment_name();

	Ref<SpineAttachment> copy();

	void set_spine_object(const SpineSkeletonDataResource *_owner, spine::Attachment *_object) override {
		if (get_spine_object()) get_spine_object()->dereference();
		_set_spine_object_internal(_owner, _object);
		if (_object) _object->reference();
	}

	// Added by the attachment-API patch. Constructs a correctly-typed wrapper
	// (SpineRegionAttachment / SpineMeshAttachment / base SpineAttachment)
	// based on the spine-cpp RTTI of the underlying attachment. Replaces the
	// repeated `memnew(SpineAttachment)` + `set_spine_object(...)` pattern
	// at every call site that returns an attachment to GDScript so consumers
	// receive a typed subclass and can call set_region() on mesh/region
	// attachments.
	static Ref<SpineAttachment> create_typed_wrapper(const SpineSkeletonDataResource *owner, spine::Attachment *attachment);
};
