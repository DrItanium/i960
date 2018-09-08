#ifndef I960_CORE_H__
#define I960_CORE_H__
#include "types.h"
namespace i960 {
	class Core {
		public:
			using RegisterWindow = NormalRegister[LocalRegisterCount];
			Core() = default;
			~Core() = default;
			/** 
			 * perform a call
			 */
			virtual void call(Ordinal address) = 0;
			virtual Ordinal load(Ordinal address) = 0;
			virtual void store(Ordinal address, Ordinal value) = 0;
		protected:
			ArithmeticControls _ac;
			TraceControl _tc;
			ProcessControls _pc;
			RegisterWindow _globalRegisters;
			RegisterWindow* _localRegisters = nullptr;
	};
} // end namespace i960
#endif // end I960_CORE_H__
