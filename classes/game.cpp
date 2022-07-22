//Here comes the definition of the game class
#include <cstdio>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "Game.h"
#include "SDL_setup.h"
#include "Level.h"
#include "IndustrialBuilding.h"
#include "MouseHandler.h"
#include "LayerHandler.h"
#include "EntityHandler.h"
#include "RenderableHandler.h"
#include "ConfigFile.h"
#include "Timer.h"

Game::Game()
    : mState(STATE::MAIN_MENU)
    , mMouse_position()
    , mAverage_fps("FPS: 0.0", { 0, 0, 0, 0 }, OVERLAY, { 255, 0, 0 }, true)
    , mVsync_enabled(false)
{
    mMain_menu = new MainMenu(this);
    mLevel_select_menu = new LevelSelectMenu(this);
    mCurrent_level = nullptr;
    const std::string vsync_flag = gConfig_file->value("video", "vsync");
    if (vsync_flag == "true") mVsync_enabled = true;
}

Game::~Game()
{
    delete mCurrent_level;
}

void Game::render_all() const
{
    gRenderables_handler->render();
}

void Game::start_game()
{
    SDL_RenderClear(gRenderer);

    SDL_RenderPresent(gRenderer);

    //loop flag
    auto quit = false;

    //frames per second timer
    Timer fps_timer;

    //the frames per second cap timer
    Timer cap_timer;

    //how many frames are counted before resetting
    int max_counted_frames = 60;
    int counted_frames = 0;
    fps_timer.start();

    while (!quit)
    {
        cap_timer.start();

        //also renders the hover window
        //mouse handler update needs to happen first
        gMouse_handler->update();
        gMouse_handler->get_mouse_position(&mMouse_position.x, &mMouse_position.y);
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            gMouse_handler->handle_event(&e);

            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        gEntity_handler->update();

        switch (mState)
        {
        case STATE::MAIN_MENU:
            break;
        case STATE::LEVEL_SELECT:
            break;
        case STATE::PLAYING:
            mCurrent_level->on_tick();

            if (mCurrent_level->is_dead() || mCurrent_level->no_lives())
            {
                mState = STATE::MAIN_MENU;
                delete mCurrent_level;
                mCurrent_level = nullptr;
            }
            break;
        case STATE::EXITING:
            quit = true;
            break;
        default:
            throw std::exception("Unknown game state");
        }

        this->render_all();

        gLayer_handler->present();

        //only correct fps manually if vsync is disabled
        if (!mVsync_enabled) {
            //if frame finished early
            const auto frame_ticks = cap_timer.get_ticks();
            if (frame_ticks < *gTicks_per_frame)
            {
                //wait remaining time
                SDL_Delay(*gTicks_per_frame - frame_ticks);
            }
        }

        ++counted_frames;
        if (counted_frames >= max_counted_frames)
        {
            //show fps over last couple of frames

            double avg_fps = counted_frames / (fps_timer.get_ticks() / 1000.f);
            if (avg_fps > 2000000) avg_fps = 0;
            std::stringstream avg_fps_text;
            avg_fps_text << std::setw(3) << avg_fps;
            mAverage_fps.set_text("FPS: " + avg_fps_text.str());

            //restart timer
            fps_timer.stop();
            fps_timer.start();
            counted_frames %= max_counted_frames;
        }
    }
}

void Game::load_level(const int level_number)
{
    if (!gConfig_file->value_exists("level" + std::to_string(level_number), "exists"))
    {
        printf("Could not load level, doesn't exist in config file!\n");
        return;
    }
    mCurrent_level = new Level(std::to_string(level_number), this);
}

void Game::set_state(const Game::STATE state)
{
    mState = state;
    switch (state)
    {
    case STATE::MAIN_MENU:
        mMain_menu->set_enabled(true);
        break;
    case STATE::PLAYING:
        mMain_menu->set_enabled(false);
        break;
    default:;
    }
}

MainMenu* Game::get_main_menu()
{
    return mMain_menu;
}

LevelSelectMenu* Game::get_level_select_menu()
{
    return mLevel_select_menu;
}
