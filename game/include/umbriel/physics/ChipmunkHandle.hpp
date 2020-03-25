#pragma once

#include <umbriel/Types.hpp>
#include <umbriel/Log.hpp>

namespace umbriel::physics
{
	template<typename T>
	class ChipmunkHandle
	{
	public:
		ChipmunkHandle(ChipmunkHandle<T> const&) = delete;

		ChipmunkHandle& operator=(ChipmunkHandle<T> const&) = delete;

		ChipmunkHandle(ChipmunkHandle&& other) noexcept
		{
			_handle = std::move(other._handle);
			_deleter = std::move(other._deleter);

			other._deleter = {};
			other._handle = nullptr;
		}

		ChipmunkHandle& operator=(ChipmunkHandle&& other) noexcept
		{
			_handle = std::move(other._handle);
			_deleter = std::move(other._deleter);

			other._deleter = {};
			other._handle = nullptr;

			return *this;
		}

		inline virtual ~ChipmunkHandle()
		{
			if (_handle != nullptr)
			{
				_deleter(_handle);
				_handle = nullptr;
			}
		}

		inline T* handle() const
		{ return _handle; }

		inline T* handle()
		{ return _handle; }

	protected:
		inline explicit ChipmunkHandle(T* handle, function<void(T*)> const& deleter)
				: _deleter{deleter}, _handle{handle}
		{}

	private:
		function<void(T*)> _deleter{};
		T* _handle{nullptr};
	};
}
