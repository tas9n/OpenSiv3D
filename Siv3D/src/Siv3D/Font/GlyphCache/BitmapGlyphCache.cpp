﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2021 Ryo Suzuki
//	Copyright (c) 2016-2021 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include <Siv3D/TextureRegion.hpp>
# include <Siv3D/Math.hpp>
# include "BitmapGlyphCache.hpp"

namespace s3d
{
	RectF BitmapGlyphCache::draw(const FontData& font, const StringView s, const Vec2& pos, const double size, const ColorF& color, const double lineHeightScale)
	{
		return draw(font, s, pos, size, color, false, lineHeightScale);
	}

	RectF BitmapGlyphCache::drawBase(const FontData& font, const StringView s, const Vec2& pos, const double size, const ColorF& color, const double lineHeightScale)
	{
		return draw(font, s, pos, size, color, true, lineHeightScale);
	}

	RectF BitmapGlyphCache::region(const FontData& font, const StringView s, const Vec2& pos, const double size, const double lineHeightScale)
	{
		return region(font, s, pos, size, false, lineHeightScale);
	}

	RectF BitmapGlyphCache::regionBase(const FontData& font, const StringView s, const Vec2& pos, const double size, const double lineHeightScale)
	{
		return region(font, s, pos, size, true, lineHeightScale);
	}

	void BitmapGlyphCache::setBufferWidth(int32)
	{
		// do nothing
	}

	int32 BitmapGlyphCache::getBufferWidth() const noexcept
	{
		return 0;
	}

	bool BitmapGlyphCache::preload(const FontData& font, const StringView s)
	{
		return prerender(font, s, font.getGlyphClusters(s));
	}

	const Texture& BitmapGlyphCache::getTexture() const noexcept
	{
		return m_texture;
	}

	bool BitmapGlyphCache::prerender(const FontData& font, const StringView s, const Array<GlyphCluster>& clusters)
	{
		bool hasDirty = false;

		for (const auto& cluster : clusters)
		{
			if ((cluster.glyphIndex == 0)
				&& IsControl(s[cluster.pos]))
			{
				continue;
			}

			if (m_glyphTable.contains(cluster.glyphIndex))
			{
				continue;
			}

			const BitmapGlyph glyph = font.renderBitmapByGlyphIndex(cluster.glyphIndex);

			if (m_glyphTable.contains(glyph.glyphIndex))
			{
				continue;
			}

			if (not CacheGlyph(font, glyph.image, glyph, m_buffer, m_glyphTable))
			{
				return false;
			}

			hasDirty = true;
		}

		if (hasDirty)
		{
			m_texture = Texture{ m_buffer.image };
		}

		return true;
	}

	RectF BitmapGlyphCache::draw(const FontData& font, const StringView s, const Vec2& pos, const double size, const ColorF& color, const bool usebasePos, const double lineHeightScale)
	{
		const Array<GlyphCluster> clusters = font.getGlyphClusters(s);

		if (not prerender(font, s, clusters))
		{
			return RectF{ 0 };
		}

		const auto& prop = font.getProperty();
		const double scale = (size / prop.fontPixelSize);
		const bool noScaling = (size == prop.fontPixelSize);
		const Vec2 basePos{ pos };
		Vec2 penPos{ basePos };
		int32 lineCount = 1;
		double xMax = basePos.x;

		for (const auto& cluster : clusters)
		{
			if (ProcessControlCharacter(s[cluster.pos], penPos, lineCount, basePos, scale, lineHeightScale, prop))
			{
				xMax = Max(xMax, penPos.x);
				continue;
			}

			const auto& cache = m_glyphTable.find(cluster.glyphIndex)->second;
			{
				const TextureRegion textureRegion = m_texture(cache.textureRegionLeft, cache.textureRegionTop, cache.textureRegionWidth, cache.textureRegionHeight);
				const Vec2 posOffset = usebasePos ? cache.info.getBase(scale) : cache.info.getOffset(scale);
				const Vec2 drawPos = (penPos + posOffset);

				if (noScaling)
				{
					textureRegion
						.draw(Math::Round(drawPos), color);
				}
				else
				{
					textureRegion
						.scaled(scale)
						.draw(drawPos, color);
				}
			}

			penPos.x += (cache.info.xAdvance * scale);
			xMax = Max(xMax, penPos.x);
		}

		const Vec2 topLeft = (usebasePos ? pos.movedBy(0, -prop.ascender) : pos);
		return{ topLeft, (xMax - basePos.x), (lineCount * prop.height() * scale * lineHeightScale) };
	}

	RectF BitmapGlyphCache::region(const FontData& font, const StringView s, const Vec2& pos, const double size, const bool usebasePos, const double lineHeightScale)
	{
		const Array<GlyphCluster> clusters = font.getGlyphClusters(s);

		if (not prerender(font, s, clusters))
		{
			return RectF{ 0 };
		}

		const auto& prop = font.getProperty();
		const double scale = (size / prop.fontPixelSize);
		const Vec2 basePos{ pos };
		Vec2 penPos{ basePos };
		int32 lineCount = 1;
		double xMax = basePos.x;

		for (const auto& cluster : clusters)
		{
			if (ProcessControlCharacter(s[cluster.pos], penPos, lineCount, basePos, scale, lineHeightScale, prop))
			{
				xMax = Max(xMax, penPos.x);
				continue;
			}

			const auto& cache = m_glyphTable.find(cluster.glyphIndex)->second;
			penPos.x += (cache.info.xAdvance * scale);
			xMax = Max(xMax, penPos.x);
		}

		const Vec2 topLeft = (usebasePos ? pos.movedBy(0, -prop.ascender) : pos);
		return{ topLeft, (xMax - basePos.x), (lineCount * prop.height() * scale * lineHeightScale) };
	}
}
