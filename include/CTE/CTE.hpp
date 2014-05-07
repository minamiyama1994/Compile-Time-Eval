#ifndef CTE_CTE_HPP
#define CTE_CTE_HPP
#include<string>
#include<type_traits>
#include"boost/lexical_cast.hpp"
#include"sprout/string.hpp"
#include"sprout/type/string.hpp"
#include"FTMP/list.hpp"
#include"FTMP/integral.hpp"
#include"FTMP/at.hpp"
namespace cte
{
	namespace detail
	{
		template < typename >
		struct double_literal ;
		template < typename >
		struct int_literal ;
		template < typename >
		struct string_literal ;
		struct add_operator ;
		struct sub_operator ;
		struct mul_operator ;
		struct div_operator ;
		struct lp ;
		struct rp ;
		template < typename , typename >
		struct add_expression ;
		template < typename , typename >
		struct sub_expression ;
		template < typename , typename >
		struct mul_expression ;
		template < typename , typename >
		struct div_expression ;
		template < typename >
		struct parse ;
		template < typename >
		struct parse_mul_div ;
		template < typename >
		struct parse_add_sub ;
		template < typename >
		struct evaluate ;
		template < typename l >
		struct evaluate < ftmp::list < l > >
			: evaluate < l >
		{
		} ;
		template < typename ... str >
		struct evaluate < string_literal < ftmp::list < str ... > > >
		{
			static auto func ( ) -> std::string
			{
				return { str::value ... } ;
			}
		} ;
		template < typename ... ch >
		struct evaluate < double_literal < ftmp::list < ch ... > > >
		{
			static auto func ( ) -> double
			{
				return boost::lexical_cast < double > ( std::string { ch::value ... } ) ;
			}
		} ;
		template < typename ... ch >
		struct evaluate < int_literal < ftmp::list < ch ... > > >
		{
			static auto func ( ) -> int
			{
				return boost::lexical_cast < int > ( std::string { ch::value ... } ) ;
			}
		} ;
		template < typename lhs , typename rhs >
		struct evaluate < add_expression < lhs , rhs > >
		{
			static auto func ( )
			{
				return evaluate < lhs >::func ( ) + evaluate < rhs >::func ( ) ;
			}
		} ;
		template < typename lhs , typename rhs >
		struct evaluate < sub_expression < lhs , rhs > >
		{
			static auto func ( )
			{
				return evaluate < lhs >::func ( ) - evaluate < rhs >::func ( ) ;
			}
		} ;
		template < typename lhs , typename rhs >
		struct evaluate < mul_expression < lhs , rhs > >
		{
			static auto func ( )
			{
				return evaluate < lhs >::func ( ) * evaluate < rhs >::func ( ) ;
			}
		} ;
		template < typename lhs , typename rhs >
		struct evaluate < div_expression < lhs , rhs > >
		{
			static auto func ( )
			{
				return evaluate < lhs >::func ( ) / evaluate < rhs >::func ( ) ;
			}
		} ;
		template < typename ... parsed , typename ... tokens >
		struct parse < ftmp::list < ftmp::list < parsed ... > , ftmp::list < rp , tokens ... > > >
			: ftmp::list < ftmp::list < parsed ... >  , ftmp::list < tokens ... > >
		{
		} ;
		template < typename ... parsed , typename ... tokens >
		struct parse < ftmp::list < ftmp::list < parsed ... > , ftmp::list < lp , tokens ... > > >
			: parse
			<
				ftmp::list
				<
					ftmp::list
					<
						parsed ... ,
						typename ftmp::at < typename parse < ftmp::list < ftmp::list < > , ftmp::list < tokens ... > > >::type , ftmp::integral < int , 0 > >::type
					> , 
					typename ftmp::at < typename parse < ftmp::list < ftmp::list < > , ftmp::list < tokens ... > > >::type , ftmp::integral < int , 1 > >::type
				>
			>
		{
		} ;
		template < typename ... parsed , typename head , typename ... tokens >
		struct parse < ftmp::list < ftmp::list < parsed ... > , ftmp::list < head , tokens ... > > >
			: parse < ftmp::list < ftmp::list < parsed ... , ftmp::list < head > > , ftmp::list < tokens ... > > >
		{
		} ;
		template < typename ... parsed , typename head , typename ... tokens >
		struct parse < ftmp::list < ftmp::list < parsed ... > , ftmp::list < ftmp::list < head > , tokens ... > > >
			: parse < ftmp::list < ftmp::list < parsed ... > , ftmp::list < head , tokens ... > > >
		{
		} ;
		template < typename ... parsed >
		struct parse < ftmp::list < ftmp::list < parsed ... > , ftmp::list < > > >
			: parse_mul_div < ftmp::list < ftmp::list < > , ftmp::list < parsed ... > > >
		{
		} ;
		template < typename ... parsed , typename first , typename second , typename ... tokens >
		struct parse_mul_div < ftmp::list < ftmp::list < parsed ... > , ftmp::list < first , ftmp::list < mul_operator > , second , tokens ... > > >
			: parse_mul_div
			<
				ftmp::list
				<
					ftmp::list < parsed ... > ,
					ftmp::list < ftmp::list < mul_expression < typename parse < ftmp::list < ftmp::list < > , first > >::type , typename parse < ftmp::list < ftmp::list < > , second > >::type > > , tokens ... >
				>
			>
		{
		} ;
		template < typename ... parsed , typename first , typename second , typename ... tokens >
		struct parse_mul_div < ftmp::list < ftmp::list < parsed ... > , ftmp::list < first , ftmp::list < div_operator > , second , tokens ... > > >
			: parse_mul_div
			<
				ftmp::list
				<
					ftmp::list < parsed ... > ,
					ftmp::list < ftmp::list < div_expression < typename parse < ftmp::list < ftmp::list < > , first > >::type , typename parse < ftmp::list < ftmp::list < > , second > >::type > > , tokens ... >
				>
			>
		{
		} ;
		template < typename ... parsed , typename first , typename op , typename second , typename ... tokens >
		struct parse_mul_div < ftmp::list < ftmp::list < parsed ... > , ftmp::list < first , op , second , tokens ... > > >
			: parse_mul_div
			<
				ftmp::list
				<
					ftmp::list < parsed ... , typename parse_mul_div < ftmp::list < ftmp::list < > , first > >::type , op > ,
					ftmp::list < second , tokens ... >
				>
			>
		{
		} ;
		template < typename ... parsed , typename end >
		struct parse_mul_div < ftmp::list < ftmp::list < parsed ... > , ftmp::list < end > > >
			: parse_add_sub
			<
				ftmp::list < parsed ... , end >
			>
		{
		} ;
		template < typename first , typename second , typename ... tokens >
		struct parse_add_sub < ftmp::list < first , ftmp::list < add_operator > , second , tokens ... > >
			: parse_add_sub
			<
				ftmp::list < add_expression < typename parse_add_sub < first >::type , typename parse_add_sub < second >::type > , tokens ... >
			>
		{
		} ;
		template < typename first , typename second , typename ... tokens >
		struct parse_add_sub < ftmp::list < first , ftmp::list < sub_operator > , second , tokens ... > >
			: parse_add_sub
			<
				ftmp::list < sub_expression < typename parse_add_sub < ftmp::list < ftmp::list < > , first > >::type , typename parse_add_sub < ftmp::list < ftmp::list < > , second > >::type > , tokens ... >
			>
		{
		} ;
		template < typename end >
		struct parse_add_sub < ftmp::list < end > >
			: evaluate < end >
		{
			using type = ftmp::list < end > ;
		} ;
		template < typename end >
		struct parse_add_sub
			: evaluate < end >
		{
			using type = ftmp::list < end > ;
		} ;
		template < typename >
		struct pre_expression_to_expression ;
		template < char ... ch >
		struct pre_expression_to_expression < sprout::types::basic_string < char , ch ... > >
			: ftmp::list < ftmp::integral < char , ch > ... >
		{
		} ;
		template < typename , typename >
		struct eval ;
		template < typename , typename , typename >
		struct eval_number ;
		template < typename , typename , typename >
		struct eval_int ;
		template < typename , typename , typename >
		struct eval_double ;
		template < typename , typename , typename >
		struct eval_string ;
		template < typename ... T >
		struct eval < ftmp::list < T ... > , ftmp::list < > >
			: parse < ftmp::list < ftmp::list < > , ftmp::list < T ... > > >
		{
		} ;
#define pass(ch) \
		template < typename ... T1 , typename ... T2 > \
		struct eval < ftmp::list < T1 ... > , ftmp::list < ftmp::integral < char , ch > , T2 ... > > \
			: eval < ftmp::list < T1 ... > , ftmp::list < T2 ... > > \
		{ \
		}
		pass( ' ' ) ;
		pass( '\t' ) ;
		pass( '\r' ) ;
		pass( '\n' ) ;
#undef pass
#define to_number(ch) \
		template < typename ... T1 , typename ... T2 > \
		struct eval < ftmp::list < T1 ... > , ftmp::list < ftmp::integral < char , ch > , T2 ... > > \
			: eval_number < ftmp::list < T1 ... > , ftmp::list < > , ftmp::list < ftmp::integral < char , ch > , T2 ... > > \
		{ \
		}
		to_number ( '0' ) ;
		to_number ( '1' ) ;
		to_number ( '2' ) ;
		to_number ( '3' ) ;
		to_number ( '4' ) ;
		to_number ( '5' ) ;
		to_number ( '6' ) ;
		to_number ( '7' ) ;
		to_number ( '8' ) ;
		to_number ( '9' ) ;
		to_number ( '.' ) ;
#undef to_number
		template < typename ... T1 , typename ... T2 >
		struct eval < ftmp::list < T1 ... > , ftmp::list < ftmp::integral < char , '\"' > , T2 ... > >
			: eval_string < ftmp::list < T1 ... > , ftmp::list < > , ftmp::list < T2 ... > >
		{
		} ;
#define eval_operator(op1,op2) \
		template < typename ... T1 , typename ... T2 > \
		struct eval < ftmp::list < T1 ... > , ftmp::list < ftmp::integral < char , op1 > , T2 ... > > \
			: eval < ftmp::list < T1 ... , op2 > , ftmp::list < T2 ... > > \
		{ \
		}
		eval_operator ( '+' , add_operator ) ;
		eval_operator ( '-' , sub_operator ) ;
		eval_operator ( '*' , mul_operator ) ;
		eval_operator ( '/' , div_operator ) ;
		eval_operator ( '(' , lp ) ;
		eval_operator ( ')' , rp ) ;
#undef eval_operator
#define continue(ch) \
		template < typename ... T1 , typename ... T2 , typename ... T3 > \
		struct eval_number < ftmp::list < T1 ... > , ftmp::list < T2 ... > , ftmp::list < ftmp::integral < char , ch > , T3 ... > > \
			: eval_number < ftmp::list < T1 ... > , ftmp::list < T2 ... , ftmp::integral < char , ch > > , ftmp::list < T3 ... > > \
		{ \
		}
		continue ( '0' ) ;
		continue ( '1' ) ;
		continue ( '2' ) ;
		continue ( '3' ) ;
		continue ( '4' ) ;
		continue ( '5' ) ;
		continue ( '6' ) ;
		continue ( '7' ) ;
		continue ( '8' ) ;
		continue ( '9' ) ;
#undef continue
		template < typename ... T1 , typename ... T2 , typename ... T3 >
		struct eval_number < ftmp::list < T1 ... > , ftmp::list < T2 ... > , ftmp::list < ftmp::integral < char , '.' > , T3 ... > >
			: eval_double < ftmp::list < T1 ... > , ftmp::list < T2 ... , ftmp::integral < char , '.' > > , ftmp::list < T3 ... > >
		{
		} ;
		template < typename ... T1 , typename ... T2 , typename ... T3 >
		struct eval_number < ftmp::list < T1 ... > , ftmp::list < T2 ... > , ftmp::list < T3 ... > >
			: eval_int < ftmp::list < T1 ... > , ftmp::list < T2 ... > , ftmp::list < T3 ... > >
		{
		} ;
#define continue(ch) \
		template < typename ... T1 , typename ... T2 , typename ... T3 > \
		struct eval_double < ftmp::list < T1 ... > , ftmp::list < T2 ... > , ftmp::list < ftmp::integral < char , ch > , T3 ... > > \
			: eval_double < ftmp::list < T1 ... > , ftmp::list < T2 ... , ftmp::integral < char , ch > > , ftmp::list < T3 ... > > \
		{ \
		}
		continue ( '0' ) ;
		continue ( '1' ) ;
		continue ( '2' ) ;
		continue ( '3' ) ;
		continue ( '4' ) ;
		continue ( '5' ) ;
		continue ( '6' ) ;
		continue ( '7' ) ;
		continue ( '8' ) ;
		continue ( '9' ) ;
#undef continue
		template < typename ... T1 , typename ... T2 , typename ... T3 >
		struct eval_double < ftmp::list < T1 ... > , ftmp::list < T2 ... > , ftmp::list < T3 ... > > 
			: eval < ftmp::list < T1 ... , double_literal < ftmp::list < T2 ... > > > , ftmp::list < T3 ... > >
		{
		} ;
		template < typename ... T1 , typename ... T2 , typename ... T3 >
		struct eval_int < ftmp::list < T1 ... > , ftmp::list < T2 ... > , ftmp::list < T3 ... > > 
			: eval < ftmp::list < T1 ... , int_literal < ftmp::list < T2 ... > > > , ftmp::list < T3 ... > >
		{
		} ;
		template < typename ... T1 , typename ... T2 , typename head , typename ... T3 >
		struct eval_string < ftmp::list < T1 ... > , ftmp::list < T2 ... > , ftmp::list < head , T3 ... > >
			: eval_string < ftmp::list < T1 ... > , ftmp::list < T2 ... , head > , ftmp::list < T3 ... > >
		{
		} ;
		template < typename ... T1 , typename ... T2 , typename head , typename ... T3 >
		struct eval_string < ftmp::list < T1 ... > , ftmp::list < T2 ... > , ftmp::list < ftmp::integral < char , '\\' > , head , T3 ... > >
			: eval_string < ftmp::list < T1 ... > , ftmp::list < T2 ... , head > , ftmp::list < T3 ... > >
		{
		} ;
		template < typename ... T1 , typename ... T2 , typename ... T3 >
		struct eval_string < ftmp::list < T1 ... > , ftmp::list < T2 ... > , ftmp::list < ftmp::integral < char , '\"' > , T3 ... > >
			: eval < ftmp::list < T1 ... , string_literal < ftmp::list < T2 ... > > > , ftmp::list < T3 ... > >
		{
		} ;
	}
	template < typename expression_type >
	auto eval ( )
	{
		return detail::eval < ftmp::list < > , expression_type >::func ( ) ;
	}
}
#define CTE_EVAL(expression) \
	[ ] ( ) \
	{ \
		SPROUT_TYPES_STRING_TYPEDEF ( sprout::to_string( expression ) , SPROUT_PP_CAT ( pre_expression_type , __LINE__) ) ; \
		using SPROUT_PP_CAT ( expression_type , __LINE__ ) = typename cte::detail::pre_expression_to_expression < SPROUT_PP_CAT ( pre_expression_type , __LINE__ )>::type ; \
		return cte::eval < SPROUT_PP_CAT ( expression_type , __LINE__ ) > ( ) ; \
	} ( )
#endif