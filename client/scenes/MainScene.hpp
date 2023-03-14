#pragma once

#include "Scene.hpp"

#include <QPushButton>

namespace babel {
    class MainScene : public Scene {
        public:
            ~MainScene() override;
    
            std::string getName() override;
            void load(std::shared_ptr<QWidget> parent) override;
            void display() override;
            void clear() override;
            void refresh() override;

        private:
            std::shared_ptr<QWidget> _parent = nullptr;
            std::unique_ptr<QPushButton> _button = nullptr;
    };

}
