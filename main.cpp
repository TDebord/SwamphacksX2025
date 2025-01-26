#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

struct Particle {
    float xVel;
    float yVel;
    float xLoc;
    float yLoc;
    int size;
    bool is_indep;

    Particle() {
        xVel = 0.0f;
        yVel = 0.0f;
        xLoc = 0.0f;
        yLoc = 0.0f;
        size = 5;
        is_indep = true;
    }

    Particle(float input_xVel, float input_yVel, float input_xLoc, float input_yLoc) {
        xVel = input_xVel;
        yVel = input_yVel;
        xLoc = input_xLoc;
        yLoc = input_yLoc;
        size = 5;
        is_indep = true;
    }

    void Display() {
        cout << "xLocation: " << xLoc << endl;
        cout << "yLocation: " << yLoc << endl;
        cout << "xVelocity: " << xVel << endl;
        cout << "yVelocity: " << yVel << endl;
    }
};

vector<Particle> Create_Particle_Vector(int particle_count, sf::RenderWindow* window) {
    srand(time(nullptr));
    int randomX, randomY;
    vector<Particle> return_vector;
    for (int i = 0; i < particle_count; i++) {
        float xVel_float_conv;
        float yVel_float_conv;
        Particle particle;
        randomX = rand() % 500;
        randomY = rand() % 500;
//        particle.xLoc = randomX+window->getPosition().x;
//        particle.yLoc = randomY+window->getPosition().y;
        particle.xLoc = randomX;
        particle.yLoc = randomY;
        randomX = rand() % 9 + 1;
        randomY = rand() % 9 + 1;

//        cout << "random x " << randomX << endl;
//        cout << "random y " << randomY << endl;

        float randomX_float = randomX;
        float randomY_float = randomY;

        particle.xVel = randomX_float / 30;
        particle.yVel = randomY_float / 30;

//        cout << "velconx " << randomX_float << endl;
//        cout << "velcony " << randomY_float << endl;

//        cout << "creating particles at: (" << particle.xLoc << ", " << particle.yLoc << " )" << endl;
        return_vector.push_back(particle);
    }
    return return_vector;
}

int main() {
    int particle_add_val = 1;
    bool collision_state = true;
    float radius = 5.0f;
    int starting_particle_count = 30;
    int particle_count = starting_particle_count;
    int window_X_size = 490;
    int window_Y_size = 490;
    float window_X_size_float = window_X_size;
    float window_Y_size_float = window_Y_size;
    vector<vector<Particle>> particle_vector_vector;
    //vector of the form (0)(0,X,1,X,2,X,3,X,...,X) -- (1)(0,X,1,X,2,X,3,X,...,X)  which holds each corresponding container value
    // in the outer vector and then the corresponding adjacency and correlation to every other container

    int rows = 8;
    int cols = 8;
    vector<vector<int>> adj_vector(rows, vector<int>(cols));
    //initalize adj_vector w/ default values
    for(int s = 0; s < 8; s++)
    {
        adj_vector.at(s).at(0) = 0;
        adj_vector.at(s).at(1) = 99;
        adj_vector.at(s).at(2) = 1;
        adj_vector.at(s).at(3) = 99;
        adj_vector.at(s).at(4) = 2;
        adj_vector.at(s).at(5) = 99;
        adj_vector.at(s).at(6) = 3;
        adj_vector.at(s).at(7) = 99;
    }

    vector<Particle> particle_vector;
    vector<Particle> placeholder_vector;
//    Particle particle1(-0.1, -0.1, 200, 200);
//    Particle particle2(0.1, 0.1, 0,0);
//    particle_vector.push_back(particle1);
//    particle_vector.push_back(particle2);

    sf::Font font;
    if (!font.loadFromFile("../arial.ttf")) {
        return -1;
    }

    sf::RenderWindow master_window(sf::VideoMode(600, 200), "Master Menu");
//    sf::RenderWindow window1(sf::VideoMode(window_X_size, window_Y_size), "Container 1");
//    sf::RenderWindow window2(sf::VideoMode(window_X_size, window_Y_size), "Container 2");
    vector<sf::RenderWindow *> window_vector;
//    window_vector.push_back(new sf::RenderWindow(sf::VideoMode(window_X_size,window_Y_size), "Container"));

    vector<int> container_button_X;
    vector<int> container_button_Y;

    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color::Yellow);

    sf::RectangleShape add_container_button(sf::Vector2f(150, 50));
    add_container_button.setFillColor(sf::Color::Yellow);

    sf::RectangleShape col_toggle_button(sf::Vector2f(150, 50));
    col_toggle_button.setFillColor(sf::Color::Yellow);

    sf::RectangleShape add_five_button(sf::Vector2f(50, 50));
    add_five_button.setFillColor(sf::Color::Yellow);

    sf::RectangleShape add_one_button(sf::Vector2f(50, 50));
    add_one_button.setFillColor(sf::Color::Yellow);

    sf::RectangleShape container_button(sf::Vector2f(55.5, 55.5));
    container_button.setFillColor(sf::Color::Blue);

    sf::RectangleShape container_space(sf::Vector2f(250, 167));
    container_space.setFillColor(sf::Color::Black);
    container_space.setOutlineThickness(5);

    sf::RectangleShape valid_line_vert(sf::Vector2f(5, 500));
    valid_line_vert.setFillColor(sf::Color::Green);

    sf::RectangleShape valid_line_horz(sf::Vector2f(500,5));
    valid_line_horz.setFillColor(sf::Color::Green);


    sf::Transform right_adj_translate;
    right_adj_translate.translate(485.f, 0.f);

    sf::Transform downward_adj_translate;
    downward_adj_translate.translate(0.f, 485.f);

//    container_space.setOutlineColor(sf::Color::White);
    //=========================================== RUNTIME LOOP ===========================================
    master_window.setPosition({1640, 10});
    while (master_window.isOpen()) {
        sf::Event event;
        while (master_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                master_window.close();
            }
        }

        master_window.clear();

        //clear screen for all containers
        if (not window_vector.empty()) {
            for (int s = 0; s < window_vector.size(); s++) {
                window_vector.at(s)->clear();
            }
        }


        static bool lock_click;
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left && not lock_click)
            {
//                cout << "MOUSE POSITION RELATIVE TO MASTER WINDOW: (" << sf::Mouse::getPosition(master_window).x << ", "
//                     << sf::Mouse::getPosition(master_window).y << ")" << endl;
//                cout << "MOUSE POSITION RELATIVE TO GLOBAL POSITI: (" << sf::Mouse::getPosition().x << ", " << sf::Mouse::getPosition().y << ")" << endl;
                lock_click = true;
                if (sf::Mouse::getPosition(master_window).x > 50 and sf::Mouse::getPosition(master_window).x < 200
                    and sf::Mouse::getPosition(master_window).y > 70 and
                    sf::Mouse::getPosition(master_window).y < 120)
                {
//                    cout << "SPAWN NEW CONTAINER" << endl;
                    window_vector.push_back(
                            new sf::RenderWindow(sf::VideoMode(window_X_size, window_Y_size), "Container"));
                    container_button_X.push_back(0);
                    container_button_Y.push_back(0);
                    particle_vector_vector.push_back(placeholder_vector);
                }

                if (sf::Mouse::getPosition(master_window).x > 50 and sf::Mouse::getPosition(master_window).x < 200
                    and sf::Mouse::getPosition(master_window).y > 130 and
                    sf::Mouse::getPosition(master_window).y < 180)
                {
//                    cout << "TOGGLE COLLISION" << endl;
                    if(collision_state)
                    {
                        collision_state=false;
                    }
                    else collision_state=true;
                }

                if (sf::Mouse::getPosition(master_window).x > 65 and sf::Mouse::getPosition(master_window).x < 115
                    and sf::Mouse::getPosition(master_window).y > 10 and
                    sf::Mouse::getPosition(master_window).y < 60)
                {
//                    cout << "+1" << endl;
                    particle_add_val = 1;

                }

                if (sf::Mouse::getPosition(master_window).x > 135 and sf::Mouse::getPosition(master_window).x < 185
                    and sf::Mouse::getPosition(master_window).y > 10 and
                    sf::Mouse::getPosition(master_window).y < 60)
                {
//                    cout << "+5" << endl;
                    particle_add_val = 5;
                }

                if (not window_vector.empty()) {
                    for (int s = 0; s < window_vector.size(); s++) {
                        if (sf::Mouse::getPosition(master_window).x - container_button_X.at(s) > 0 and
                            sf::Mouse::getPosition(master_window).x - container_button_X.at(s) < 55.5 and
                            sf::Mouse::getPosition(master_window).y - container_button_Y.at(s) > 0 and
                            sf::Mouse::getPosition(master_window).y - container_button_Y.at(s) < 55.5) {
//                            cout << "within bounds" << endl;
//                            cout << "container number " << s << " selected" << endl;
                            window_vector.at(s)->setActive(true);
                            vector<Particle> temp_vector = Create_Particle_Vector(particle_add_val, window_vector.at(s));
                            for(int m = 0; m < temp_vector.size(); m++)
                            {
                                particle_vector_vector.at(s).push_back(temp_vector.at(m));
                            }
//                            cout << "POSITION OF RELEVANT WINDOW: " << window_vector.at(s)->getPosition().x << ", " << window_vector.at(s)->getPosition().y;
                        }
                    }
                }
            }
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) lock_click = false;
        }

        container_space.setPosition(236, 19);
        master_window.draw(container_space);

        add_container_button.setPosition(50, 70);
        master_window.draw(add_container_button);

        col_toggle_button.setPosition(50, 130);
        master_window.draw(col_toggle_button);

        add_one_button.setPosition(65, 10);
        master_window.draw(add_one_button);

        add_five_button.setPosition(135, 10);
        master_window.draw(add_five_button);

        //translates window vectors positions to container buttons position
        if (not window_vector.empty()) {
            for (int s = 0; s < window_vector.size(); s++) {
                container_button_X.at(s) = (((window_vector.at(s)->getPosition().x) / 9) + 236);
                container_button_Y.at(s) = (((window_vector.at(s)->getPosition().y) / 9) + 20);
            }
        }

//        if(not window_vector.empty())
//        {
//            cout << window_vector.at(0)->getPosition().x << endl;
//            cout << window_vector.at(0)->getPosition().y << endl;
//        }

        //container UI blue square creation
        if (not window_vector.empty()) {
            for (int s = 0; s < window_vector.size(); s++) {
                container_button.setPosition(container_button_X.at(s), container_button_Y.at(s));
                master_window.draw(container_button);

                string container_num_text = "C#:" + to_string(s);
                sf::Text container_num(container_num_text, font, 18);
                container_num.setFillColor(sf::Color::White);
                container_num.setPosition(container_button_X.at(s), container_button_Y.at(s));
                master_window.draw(container_num);

                int container_button_X_offset = container_button_X.at(s);
                int container_button_Y_offset = container_button_Y.at(s)+30;

                string r_p_n_text = "P#:" + to_string(particle_vector_vector.at(s).size());
                sf::Text relative_particle_num(r_p_n_text, font, 18);
                relative_particle_num.setFillColor(sf::Color::White);
                relative_particle_num.setPosition(container_button_X_offset, container_button_Y_offset);
                master_window.draw(relative_particle_num);
            }
        }


        //Add container button
        sf::Text frame_text("Add Container", font, 18);
        frame_text.setFillColor(sf::Color::Black);
        frame_text.setPosition(52, 85);
        master_window.draw(frame_text);

        sf::Text frame_text_2("Collide State", font, 18);
        frame_text_2.setFillColor(sf::Color::Black);
        frame_text_2.setPosition(52, 145);
        master_window.draw(frame_text_2);

        sf::Text part_num_text_1("+1", font, 27);
        part_num_text_1.setFillColor(sf::Color::Black);
        part_num_text_1.setPosition(70, 15);
        master_window.draw(part_num_text_1);

        sf::Text part_num_text_2("+5", font, 27);
        part_num_text_2.setFillColor(sf::Color::Black);
        part_num_text_2.setPosition(140, 15);
        master_window.draw(part_num_text_2);

        if(collision_state)
        {
            sf::Text frame_text_3("(T)", font, 18);
            frame_text_3.setFillColor(sf::Color::White);
            frame_text_3.setPosition(10, 145);
            master_window.draw(frame_text_3);
        }
        else
        {
            sf::Text frame_text_3("(F)", font, 18);
            frame_text_3.setFillColor(sf::Color::White);
            frame_text_3.setPosition(10, 145);
            master_window.draw(frame_text_3);
        }

        if(particle_add_val==5)
        {
            sf::Text frame_text_4("(+5)", font, 18);
            frame_text_4.setFillColor(sf::Color::White);
            frame_text_4.setPosition(10, 20);
            master_window.draw(frame_text_4);
        }
        if(particle_add_val==1)
        {
            sf::Text frame_text_5("(+5)", font, 18);
            frame_text_5.setFillColor(sf::Color::White);
            frame_text_5.setPosition(10, 20);
            master_window.draw(frame_text_5);
        }

//        cout << window_vector.size() << endl;

        //adjacency handler test 1
//        if (window_vector.size()==2)
//        {
//            cout << window_vector.at(0)->getPosition().x - window_vector.at(1)->getPosition().x << endl;
//            //right adjacency
//            if(window_vector.at(0)->getPosition().x - window_vector.at(1)->getPosition().x < -485 and window_vector.at(0)->getPosition().x - window_vector.at(1)->getPosition().x > -505)
//            {
//                cout << "FLAG1" << endl;
//                window_vector.at(1)->draw(valid_line_vert);
//            }
//            //left adjacency
//            if(window_vector.at(0)->getPosition().x - window_vector.at(1)->getPosition().x < 505 and window_vector.at(0)->getPosition().x - window_vector.at(1)->getPosition().x > 485)
//            {
//                cout << "FLAG2" << endl;
//                window_vector.at(0)->draw(valid_line_vert);
////                window_vector.at(0)->draw(valid_line_vert);
//            }
//        }

//          WIP ADJACENCY CHECKER
        if(not window_vector.empty() and window_vector.size() >= 2)
        {
            for(int s = 0; s < window_vector.size(); s++)
            {
                for(int j = 0; j < window_vector.size(); j++)
                {
                    //reset default vector values
                    adj_vector.at(s).at(0) = 0;
                    adj_vector.at(s).at(2) = 1;
                    adj_vector.at(s).at(4) = 2;
                    adj_vector.at(s).at(6) = 3;

                    if(s!=j)
                    {
                        //right adjacency
                        if(window_vector.at(s)->getPosition().x - window_vector.at(j)->getPosition().x < -485
                           and window_vector.at(s)->getPosition().x - window_vector.at(j)->getPosition().x > -505
                           and abs(window_vector.at(s)->getPosition().y - window_vector.at(j)->getPosition().y) < 15)
                        {
                            adj_vector.at(s).at(5) = j;
                            window_vector.at(s)->draw(valid_line_vert, right_adj_translate);
//                            cout << "CONTAINER " << s << " right adjacent to CONTAINER " << j << endl;
                        }
//                        else
//                        {
//                            adj_vector.at(s).at(5) = 99;
//                        }
                        //left adjacency
                        if(window_vector.at(s)->getPosition().x - window_vector.at(j)->getPosition().x < 505
                           and window_vector.at(s)->getPosition().x - window_vector.at(j)->getPosition().x> 485
                           and abs(window_vector.at(s)->getPosition().y - window_vector.at(j)->getPosition().y) < 15)
                        {
                            adj_vector.at(s).at(1) = j;
                            window_vector.at(s)->draw(valid_line_vert);
//                            cout << "CONTAINER " << s << " left adjacent to CONTAINER " << j << endl;
                        }
//                        else
//                        {
//                            adj_vector.at(s).at(1) = 99;
//                        }
                        //DOWNWARD AND UPWARD MAY NEED TO BE SWITCHED
                        //upward adjacency
                        if(window_vector.at(s)->getPosition().y - window_vector.at(j)->getPosition().y < 505
                           and window_vector.at(s)->getPosition().y - window_vector.at(j)->getPosition().y > 485
                           and abs(window_vector.at(s)->getPosition().x - window_vector.at(j)->getPosition().x) < 15)
                        {
                            adj_vector.at(s).at(3) = j;
                            window_vector.at(s)->draw(valid_line_horz);
//                            cout << "CONTAINER " << s << " upward adjacent to CONTAINER " << j << endl;
                        }
//                        else
//                        {
//                            adj_vector.at(s).at(3) = 99;
//                        }
                        //downward adjacency
                        if(window_vector.at(s)->getPosition().y - window_vector.at(j)->getPosition().y < -485
                           and window_vector.at(s)->getPosition().y - window_vector.at(j)->getPosition().y > -505
                           and abs(window_vector.at(s)->getPosition().x - window_vector.at(j)->getPosition().x) < 15)
                        {
                            adj_vector.at(s).at(7) = j;
                            window_vector.at(s)->draw(valid_line_horz, downward_adj_translate);
//                            cout << "CONTAINER " << s << " downward adjacent to CONTAINER " << j << endl;
                        }
//                        else
//                        {
//                            adj_vector.at(s).at(7) = 99;
//                        }
                    }
                    //prints all values of adj array as test
//                    cout <<"START OF ADJ BLOCK" << endl;
//                    for(int n = 0; n<window_vector.size();n++)
//                    {
//                        for(int m = 0; m<8;m++)
//                        {
//                            cout << adj_vector.at(n).at(m) << endl;
//                        }
//                    }
//                    cout <<"END  OF ADJ BLOCK" << endl;
                }
            }
        }


        //game time iterator
        if (not window_vector.empty() && not particle_vector_vector.empty()) {
            for (int s = 0; s < window_vector.size(); s++) {
                if (not particle_vector_vector.at(s).empty()) {
                    for (int j = 0; j < particle_vector_vector.at(s).size(); j++) {
                        circle.setPosition(particle_vector_vector.at(s).at(j).xLoc,particle_vector_vector.at(s).at(j).yLoc);
                        sf::Text particle_num(to_string(j), font, 18);
                        particle_num.setFillColor(sf::Color::White);
                        particle_num.setPosition(particle_vector_vector.at(s).at(j).xLoc+15,particle_vector_vector.at(s).at(j).yLoc+10);
                        window_vector.at(s)->draw(particle_num);
//                        cout << particle_vector_vector.at(s).at(j).xLoc << endl;
//                        cout << particle_vector_vector.at(s).at(j).yLoc << endl;

                        if (j == 0) {
                            circle.setFillColor(sf::Color::Red);
                        }
                        if (j == 1) {
                            circle.setFillColor(sf::Color::Blue);
                        }
                        if (j == 2) {
                            circle.setFillColor(sf::Color::Green);
                        }
                        if (j == 3) {
                            circle.setFillColor(sf::Color::White);
                        }
                        window_vector.at(s)->draw(circle);

                        if (window_vector.at(s) == nullptr) {
                            cout << "Window " << s << " is nullptr!" << endl;
                        }
                        if (!window_vector.at(s)->isOpen()) {
                            std::cout << "Window " << s << " is closed." << std::endl;
                        }
                        //collision handler
                        if(collision_state)
                        {
                            for (int c = 0; c < particle_vector_vector.at(s).size(); c++) {
                                if (c != j) {
                                    if (sqrt((pow(abs(particle_vector_vector.at(s).at(j).xLoc -
                                                      particle_vector_vector.at(s).at(c).xLoc),2) +
                                              pow(abs(particle_vector_vector.at(s).at(j).yLoc -
                                                      particle_vector_vector.at(s).at(c).yLoc),
                                                  2))) < 10) {
                                        float tempX = particle_vector_vector.at(s).at(j).xVel;
                                        float tempY = particle_vector_vector.at(s).at(j).yVel;
                                        particle_vector_vector.at(s).at(j).xVel = particle_vector_vector.at(s).at(c).xVel;
                                        particle_vector_vector.at(s).at(j).yVel = particle_vector_vector.at(s).at(c).yVel;
                                        particle_vector_vector.at(s).at(c).xVel = tempX;
                                        particle_vector_vector.at(s).at(c).yVel = tempY;
                                    }
                                }
                            }
                        }

                        //WALL COLLISION HANDLER
//                        //x walls checker
//                        if (particle_vector_vector.at(s).at(j).xLoc > window_X_size_float - 8 or
//                            particle_vector_vector.at(s).at(j).xLoc < 0)
//                        {
//                            particle_vector_vector.at(s).at(j).xVel = -particle_vector_vector.at(s).at(j).xVel;
//                        }
//                        //y walls checker
//                        if (particle_vector_vector.at(s).at(j).yLoc > window_Y_size_float - 8 or
//                            particle_vector_vector.at(s).at(j).yLoc < 0)
//                        {
//                            particle_vector_vector.at(s).at(j).yVel = -particle_vector_vector.at(s).at(j).yVel;
//                        }

//                        if(window_vector.size()>=3)
//                        {
//                            cout << "left, right adj" << endl;
//                            cout << "C0: " << adj_vector.at(0).at(1) << ", " << adj_vector.at(0).at(5) << endl;
//                            cout << "C1: " << adj_vector.at(1).at(1) << ", " << adj_vector.at(1).at(5) << endl;
//                            cout << "C2: " << adj_vector.at(2).at(1) << ", " << adj_vector.at(2).at(5) << endl;
//                        }

                        //WALL COLLISION AND ADJACENCY PASS THROUGH HANDLER
                        //right wall checker
                        //remove last exception in this conditional
                        if (particle_vector_vector.at(s).at(j).xLoc > window_X_size_float - 8 and adj_vector.at(s).at(5) == 99)
                        {
                            particle_vector_vector.at(s).at(j).xVel = -particle_vector_vector.at(s).at(j).xVel;
                        }
                        else if(particle_vector_vector.at(s).at(j).xLoc > window_X_size_float - 8)
                        {

                            //grabs passing particle
                            Particle holder_particle = particle_vector_vector.at(s).at(j);
                            holder_particle.xLoc -= 475;
                            //passes particle to next container
                            particle_vector_vector.at(adj_vector.at(s).at(5)).push_back(holder_particle);
                            //removes passing particle
                            //*Memory leak btw
                            particle_vector_vector.at(s).erase(particle_vector_vector.at(s).begin()+j);
                            //must have this negative increment to control for last value passthrough, then pass to next iteration
                            --j;
                            continue;
                        }
                        //left wall checker
                        if(particle_vector_vector.at(s).at(j).xLoc < 0 and adj_vector.at(s).at(1) == 99)
                        {
                            particle_vector_vector.at(s).at(j).xVel = -particle_vector_vector.at(s).at(j).xVel;
                        }
                        else if(particle_vector_vector.at(s).at(j).xLoc < 0)
                        {

                            //grabs passing particle
                            Particle holder_particle = particle_vector_vector.at(s).at(j);
                            holder_particle.xLoc += 475;
                            //passes particle to next container
                            particle_vector_vector.at(adj_vector.at(s).at(1)).push_back(holder_particle);
                            //removes passing particle
                            //*Memory leak btw
                            particle_vector_vector.at(s).erase(particle_vector_vector.at(s).begin()+j);
                            --j;
                            continue;
                        }
                        //bottom wall checker
                        if (particle_vector_vector.at(s).at(j).yLoc > window_Y_size_float - 8 and adj_vector.at(s).at(7) == 99)
                        {
                            particle_vector_vector.at(s).at(j).yVel = -particle_vector_vector.at(s).at(j).yVel;
                        }
                        else if(particle_vector_vector.at(s).at(j).yLoc > window_Y_size_float - 8)
                        {

                            //grabs passing particle
                            Particle holder_particle = particle_vector_vector.at(s).at(j);
                            holder_particle.yLoc -= 475;
                            //passes particle to next container
                            particle_vector_vector.at(adj_vector.at(s).at(7)).push_back(holder_particle);
                            //removes passing particle
                            //*Memory leak btw
                            particle_vector_vector.at(s).erase(particle_vector_vector.at(s).begin()+j);
                            --j;
                            continue;
                        }
                        //top wall checker
                        if(particle_vector_vector.at(s).at(j).yLoc < 0 and adj_vector.at(s).at(3) == 99)
                        {
                            particle_vector_vector.at(s).at(j).yVel = -particle_vector_vector.at(s).at(j).yVel;
                        }
                        else if(particle_vector_vector.at(s).at(j).yLoc < 0)
                        {

                            //grabs passing particle
                            Particle holder_particle = particle_vector_vector.at(s).at(j);
                            holder_particle.yLoc += 475;
                            //passes particle to next container
                            particle_vector_vector.at(adj_vector.at(s).at(3)).push_back(holder_particle);
                            //removes passing particle
                            //*Memory leak btw
                            particle_vector_vector.at(s).erase(particle_vector_vector.at(s).begin()+j);
                            --j;
                            continue;
                        }


                        //position updater
                        particle_vector_vector.at(s).at(j).xLoc += particle_vector_vector.at(s).at(j).xVel;
                        particle_vector_vector.at(s).at(j).yLoc += particle_vector_vector.at(s).at(j).yVel;
                    }
                }
            }
        }
        master_window.display();
        //Displays all container vectors
        if (not window_vector.empty()) {
            for (int s = 0; s < window_vector.size(); s++) {
                window_vector.at(s)->display();
            }
        }

    }
    return 0;
}
