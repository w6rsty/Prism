#include "engine/layer_stack.hpp"
#include <algorithm>

namespace prism {

LayerStack::LayerStack() {
    layerInsert_ = layers_.begin();
}

LayerStack::~LayerStack() {
    for (Layer* layer : layers_) {
        delete layer;
    }
}

void LayerStack::PushLayer(Layer* layer) {
    layerInsert_ = layers_.emplace(layerInsert_, layer);
}
void LayerStack::PushOverlay(Layer* overlay) {
    layers_.emplace_back(overlay);
}
void LayerStack::PopLayer(Layer* layer) {
    if (auto it = std::find(layers_.begin(), layers_.end(), layer); it != layers_.end()) {
        layers_.erase(it);
        layerInsert_--;
    }
}
void LayerStack::PopOverlay(Layer* overlay) {
        if (auto it = std::find(layers_.begin(), layers_.end(), overlay); it != layers_.end()) {
        layers_.erase(it);
    }
}

} // namespace prism