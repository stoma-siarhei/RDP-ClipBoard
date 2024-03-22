#ifndef _DEFAULT_PARAM_HPP_
#define _DEFAULT_PARAM_HPP_


namespace param::ports
{
	
constexpr uint16_t lisen = 3333;

constexpr uint16_t min = 2000;

constexpr uint16_t max = 10000;

} // namespace param::ports

namespace param::buffer
{
	
constexpr size_t size = 4096;
	
} // namespace param::buffer

namespace param::timer
{
	
constexpr size_t send = 800;

constexpr size_t connect = 20000;
	
} // namespace param::timer


#endif // _DEFAULT_PARAM_HPP_