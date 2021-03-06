#include "level.h"

Level::Level() {
    add(new Player(glm::vec3(0, 48.0f, 0)));
    blocks = new unsigned short**[WIDTH];
    for (unsigned int x = 0; x < WIDTH; x++) {
        blocks[x] = new unsigned short*[DEPTH];
        for (unsigned int y = 0; y < DEPTH; y++) {
            blocks[x][y] = new unsigned short[HEIGHT];
        }
    }
    
    for (unsigned int z = 0; z < HEIGHT; z++) {
        for (unsigned int y = 0; y < DEPTH; y++) {
            for (unsigned int x = 0; x < WIDTH; x++) {
                blocks[x][y][z] = 1;
            }
        }
    }
    Block::CREATE_ALL();
}

Level::~Level() {
    for (unsigned int y = 0; y < DEPTH; y++) {
        delete [] blocks[y];
    }
    delete [] blocks;
}

void Level::update() {
    for (int i = 0; i < entities.size(); i++) {
        entities[i]->update();
    }
}

void Level::add(Entity *entity) {
    entity->init(this);
    entities.push_back(entity);
}

unsigned short& Level::get_intersecting_block(Entity *entity) {
    glm::vec3 position = -entity->get_position() / Block::SIZE;
    if (position.x < 0 || position.y < 0 || position.z < 0) return NULL_BLOCK;
    if (position.x >= WIDTH || position.y >= HEIGHT || position.z >= DEPTH) return NULL_BLOCK;
    return blocks[(short) position.x][(short) position.z][(short) position.y];
}

unsigned short& Level::get_block(glm::vec3 position) {
    std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
    return NULL_BLOCK;
}

void Level::render() {
    Shader::BLOCK->enable();
    for (int z = 0; z < HEIGHT; z++) {
        for (int y = 0; y < DEPTH; y++) {
            for (int x = 0; x < WIDTH; x++) {
                Block *block = Block::air;
                switch (blocks[x][y][z]) {
                    case 1:
                        block = Block::stone;
                        break;
                    case 2:
                        block = Block::dirt;
                        break;
                }
                block->render(glm::vec3(x * Block::SIZE + 2.0f, z * Block::SIZE + 2.0f, y * Block::SIZE + 2.0f));
            }
        }
    }
    for (int i = 0; i < entities.size(); i++) {
        entities[i]->render();
    }
    Shader::BLOCK->disable();
}