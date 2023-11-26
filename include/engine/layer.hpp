#pragma once

#include <string>
#include "engine/event/event.hpp"

namespace prism {

class Layer {
protected:
    std::string debugName_;
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {};
    virtual void OnDetach() {};
    virtual void OnUpdate() {};
    virtual void OnEvent(Event& event) {};

    inline std::string GetName() const { return debugName_; }
};

} // namespace prism