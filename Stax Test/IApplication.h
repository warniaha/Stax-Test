#pragma once
struct IApplication
{
	virtual ~IApplication() noexcept = default;
	virtual void run() = 0;
};
