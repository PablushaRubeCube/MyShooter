#pragma once

class Utils
{
public:

	template <typename T>
	static T* GetPlayerComponent(AActor* Actor)
	{
		if (!Actor) return;

		const auto Component = Actor->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}

private:

};
