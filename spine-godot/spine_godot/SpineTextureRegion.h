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

#pragma once

#include "SpineCommon.h"
#include "SpineRendererObject.h"
#include <spine/Atlas.h>
#include <spine/TextureRegion.h>

#ifdef SPINE_GODOT_EXTENSION
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/canvas_texture.hpp>
#else
#include "scene/resources/texture.h"
#endif

// Wraps a spine::AtlasRegion so GDScript can build a TextureRegion from a
// runtime-loaded Godot Texture2D and assign it to an attachment via
// SpineRegionAttachment/SpineMeshAttachment.set_region(). Mirrors Unity's
// Spine.Unity.AtlasUtilities.ToAtlasRegion(Sprite) path.
//
// Lifetime: this wrapper owns the spine::AtlasRegion, spine::AtlasPage, and
// SpineRendererObject it allocates. Attachments that call set_region(this)
// receive a raw pointer to the underlying spine::AtlasRegion; the consumer
// (typically the SpineRegionAttachment / SpineMeshAttachment wrapper) must
// hold a Ref<SpineTextureRegion> for as long as any attachment references
// the region. SpineRegionAttachment and SpineMeshAttachment do this
// automatically via an internal Ref<SpineTextureRegion> member.
class SpineTextureRegion : public REFCOUNTED {
	GDCLASS(SpineTextureRegion, REFCOUNTED)

	spine::AtlasRegion *atlas_region;
	spine::AtlasPage *atlas_page;
	SpineRendererObject *renderer_object;
	Ref<Texture2D> source_texture;

protected:
	static void _bind_methods();

public:
	SpineTextureRegion();
	~SpineTextureRegion();

	// Construct a SpineTextureRegion wrapping the whole of `texture`. Sets
	// UVs to (0,0)-(1,1) and width/height to the texture's native dimensions.
	// Returns null if `texture` is invalid.
	static Ref<SpineTextureRegion> from_texture(Ref<Texture2D> texture);

	// Convenience: set a sub-region of the source texture via UV coords.
	// UVs are in [0,1] space. If you need pixel-space coords, divide by
	// texture dimensions first. Default on construction is full (0..1).
	void set_uv(float u, float v, float u2, float v2);

	float get_u();
	float get_v();
	float get_u2();
	float get_v2();

	Ref<Texture2D> get_texture();

	// Internal: used by attachment wrappers to hand spine-cpp a pointer.
	spine::AtlasRegion *get_spine_object() { return atlas_region; }
};
