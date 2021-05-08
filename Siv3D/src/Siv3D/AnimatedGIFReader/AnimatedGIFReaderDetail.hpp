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

# pragma once
# include <Siv3D/AnimatedGIFReader.hpp>
# include <Siv3D/Blob.hpp>

namespace s3d
{
	class AnimatedGIFReader::AnimatedGIFReaderDetail
	{
	public:

		AnimatedGIFReaderDetail();

		~AnimatedGIFReaderDetail();

		bool open(FilePathView path);

		bool open(IReader&& reader);

		void close();

		bool isOpen() const noexcept;

		bool read(Array<Image>& images, Array<int32>& delays, int32& duration) const;

	private:

		Blob m_blob;
	};
}
