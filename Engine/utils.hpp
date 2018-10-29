#pragma once



namespace je
{
	constexpr float Pi          = 3.14159265359f;
	constexpr float GoldenRatio = 1.61803398875f;
	
	class Lock
	{
		Lock() {  }
		~Lock() {  }
	};
	template<class T> struct always_false : std::false_type {};
}