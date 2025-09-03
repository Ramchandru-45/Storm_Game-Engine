#pragma once

#include "Storm/Core/Buffer.h"

namespace Storm
{
	class STORM_API FrameBuffer : public Buffer
	{
	public:
		FrameBuffer() = default;
		~FrameBuffer() = default;

		virtual void Bind() const override = 0;
		virtual void Release() const override = 0;
	};
}

