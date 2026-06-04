/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated April 5, 2025. Replaces all prior versions.
 *
 * Copyright (c) 2013-2025, Esoteric Software LLC
 *
 * Integration of the Spine Runtimes into software or otherwise creating
 * derivative works of the Spine Runtimes is permitted under the terms and
 * conditions of Section 2 of the Spine Editor License Agreement:
 * http://esotericsoftware.com/spine-editor-license
 *
 * Otherwise, it is permitted to integrate the Spine Runtimes into software
 * or otherwise create derivative works of the Spine Runtimes (collectively,
 * "Products"), provided that each user of the Products must obtain their own
 * Spine Editor license and redistribution of the Products in any form must
 * include this license and copyright notice.
 *
 * THE SPINE RUNTIMES ARE PROVIDED BY ESOTERIC SOFTWARE LLC "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ESOTERIC SOFTWARE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES,
 * BUSINESS INTERRUPTION, OR LOSS OF USE, DATA, OR PROFITS) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THE SPINE RUNTIMES, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "SpineAttachment.h"
#include "SpineCommon.h"
// The subclass headers are included here so copy() can return the correctly
// typed wrapper depending on the underlying spine-cpp attachment type.
#include "SpineRegionAttachment.h"
#include "SpineMeshAttachment.h"
#include <spine/RegionAttachment.h>
#include <spine/MeshAttachment.h>

void SpineAttachment::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_attachment_name"), &SpineAttachment::get_attachment_name);
	ClassDB::bind_method(D_METHOD("copy"), &SpineAttachment::copy);
}

SpineAttachment::~SpineAttachment() {
	if (get_spine_object()) get_spine_object()->dereference();
}

String SpineAttachment::get_attachment_name() {
	SPINE_CHECK(get_spine_object(), "")
	String name;
#if (VERSION_MAJOR >= 4 && VERSION_MINOR >= 5)
	name = String::utf8(get_spine_object()->getName().buffer());
#else
	name.parse_utf8(get_spine_object()->getName().buffer());
#endif
	return name;
}

Ref<SpineAttachment> SpineAttachment::copy() {
	SPINE_CHECK(get_spine_object(), nullptr)
	auto copy = &get_spine_object()->copy();
	if (!copy) return nullptr;
	return create_typed_wrapper(get_spine_owner(), copy);
}

// Static helper: construct a correctly-typed SpineAttachment subclass wrapper
// for an arbitrary spine::Attachment*. Used by all call sites that return
// attachments to GDScript (SpineSkin::get_attachment,
// SpineSkin::find_attachments_for_slot, SpineSkin::get_attachments,
// SpineSkeleton::get_attachment_by_slot_name/_index,
// SpineSlotPose::get_attachment) so that GDScript
// consumers receive the typed subclass (SpineRegionAttachment,
// SpineMeshAttachment) and can call set_region() on mesh/region attachments.
// Other attachment types (clipping, bounding box, path, point) fall through
// to the base SpineAttachment wrapper, matching pre-patch behaviour.
Ref<SpineAttachment> SpineAttachment::create_typed_wrapper(const SpineSkeletonDataResource *owner, spine::Attachment *attachment) {
	if (!attachment) return nullptr;
	Ref<SpineAttachment> attachment_ref;
	if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti)) {
		attachment_ref = Ref<SpineAttachment>(memnew(SpineRegionAttachment));
	} else if (attachment->getRTTI().isExactly(spine::MeshAttachment::rtti)) {
		attachment_ref = Ref<SpineAttachment>(memnew(SpineMeshAttachment));
	} else {
		attachment_ref = Ref<SpineAttachment>(memnew(SpineAttachment));
	}
	attachment_ref->set_spine_object(owner, attachment);
	return attachment_ref;
}
