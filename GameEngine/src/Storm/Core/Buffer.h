#pragma once

namespace Storm
{
	class STORM_API Buffer
	{
	public:
		virtual ~Buffer() = default;

		virtual void Bind() const = 0;
		virtual void Release() const = 0;
	};
}

