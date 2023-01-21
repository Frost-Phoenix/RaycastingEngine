
hpp_file = """#pragma once


class CLASS_NAME
{
    private:
        // Sprite
        sf::Sprite sprite;

        // Variables
       
        // Private functions
        
    public:
        // Constructor
        CLASS_NAME();
        ~CLASS_NAME();

        // Accesors

        // Public functions
        void update();
        void render(sf::RenderTarget* renderTarget);
};"""

cpp_file = """#include "Utils.hpp"
#include "CLASS_NAME.hpp"


// Constructor
CLASS_NAME::CLASS_NAME()
{

}

CLASS_NAME::~CLASS_NAME()
{
    
}

// Private functions

// Accesors

// Public functions
void CLASS_NAME::update()
{
    
}

void CLASS_NAME::render(sf::RenderTarget* renderTarget)
{
    renderTarget->draw(this->sprite);
}
"""


def main():
    global cpp_file, hpp_file

    class_name = input("Name of the class : ")

    hpp_file = hpp_file.replace("CLASS_NAME", class_name)

    with open(f"include/{class_name}.hpp", "w+") as f:
        f.write(hpp_file)
        f.close()

    cpp_file = cpp_file.replace("CLASS_NAME", class_name)

    with open(f"src/{class_name}.cpp", "w+") as f:
        f.write(cpp_file)
        f.close()


if __name__ == "__main__":
    main()