#include "types.h"
#include "core.h"
#include "opcodes.h"
#include <map>
#include <functional>
#include <sstream>
#include <string>

namespace i960 {
	void Core::dispatch(const Instruction& inst) noexcept {
        std::visit([this](auto&& value) {
                    using K = std::decay_t<decltype(value)>;
                    if constexpr (std::is_same_v<K, std::monostate>) {
                        generateFault(OperationFaultSubtype::InvalidOpcode);
                    } else {
                        dispatchOperation(value);
                    }
                }, inst.decode());
	}
} // end namespace i960
