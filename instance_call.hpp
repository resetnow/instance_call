#include <type_traits>
#include <functional>

template<
	typename Function,
	Function F
>
struct instance_call_helper;

template<
	typename Result,
	typename Base,
	typename... Args,
	Result(Base::* Func)(Args...)
>
struct instance_call_helper<
	Result(Base::*)(Args...),
	Func
> {
	template<typename Type>
	struct IsArgumentType {
		static constexpr bool value =
			std::is_fundamental<Type>::value ||
			std::is_pointer<Type>::value;
	};

	template<typename... Conds>
	struct ReduceAnd : std::true_type { };

	template<typename Cond, typename... Conds>
	struct ReduceAnd<Cond, Conds...> :
		std::conditional<
			Cond::value,
			ReduceAnd<Conds...>,
			std::false_type
		>::type { };

	template<typename... T>
	using ArgumentTypes = ReduceAnd<IsArgumentType<T>...>;

	static_assert(ArgumentTypes<Args...>::value, "");

	static Result call(void * ref, Args ... args) {
		Base * instance = reinterpret_cast<Base *>(ref);
		return (instance->*Func)(std::forward<Args>(args)...);
	}
};

#if __cpp_deduction_guides >= 201606
template<
	auto F
>
#else
template<
	typename Function,
	Function F
>
#endif
#if __cpp_concepts >= 201507
requires std::is_member_function_pointer<decltype(Func)>::value
#endif
class instance_call {
	using functor_t = decltype(F);

public:
	using helper = instance_call_helper<
		functor_t,
		F
	>;
};
