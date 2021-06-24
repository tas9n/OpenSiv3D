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

# include <Siv3D/ProfilerStat.hpp>
# include <Siv3D/Print.hpp>

namespace s3d
{
	void ProfilerStat::print() const
	{
		Print << U"Draw calls:\t\t\t\t" << drawCalls;
		Print << U"Triangle count:\t\t" << triangleCount;
		Print << U"Active voice:\t\t\t" << activeVoice;
		Print << U"Texture count:\t\t" << textureCount;
		Print << U"Font count:\t\t\t\t" << fontCount;
		Print << U"Audio count:\t\t\t" << audioCount;
	}
}
