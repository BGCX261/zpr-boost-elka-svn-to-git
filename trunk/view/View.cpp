#include "Controller.hpp"
#include "View.hpp"
#include "Logger.hpp"

#include <iostream>
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

namespace zpr
{
	const int View::WINDOW_WIDTH			= 800;
	const int View::WINDOW_HEIGHT			= 600;
	const int View::VISUALISATION_WIDTH		= 600;
	const int View::VISUALISATION_HEIGHT	= 600;
	const int View::ALLEGRO_EVENT_TIMEOUT	= 10;
	const double View::SCALER = (double) VISUALISATION_WIDTH / 200;

	AllegroException::AllegroException(const std::string & message) : std::runtime_error(message.c_str()) {}

	TextRectangle::TextRectangle(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, const std::string & text)
		: topLeft_(Point(x1, y1)), bottomRight_(Point(x2, y2)), color_(color), text_(text)
	{
	}

	bool TextRectangle::inside(const Point & check) const
	{
		return inRange(topLeft_.x_, bottomRight_.x_, check.x_) && inRange(topLeft_.y_, bottomRight_.y_, check.y_);
	}

	void TextRectangle::draw(const ALLEGRO_FONT * const font, const ALLEGRO_COLOR & fontColor) const
	{
		al_draw_filled_rectangle(topLeft_.x_, topLeft_.y_, bottomRight_.x_, bottomRight_.y_, color_);
		if(font)
			al_draw_text(font, fontColor, (topLeft_.x_ + bottomRight_.x_) / 2, (topLeft_.y_ + bottomRight_.y_) / 2 - font->height / 2, ALLEGRO_ALIGN_CENTRE, text_.c_str());
	}

	View::View(Controller & controller, Model & model) : controller_(controller), model_(model), display_(NULL), eventQueue_(NULL), font_(NULL), elapsedMicroseconds_(-1)					
	{
	}

	void View::initializeGraphics()
	{
		if(!al_install_system(ALLEGRO_VERSION_INT, NULL))
		//if(!al_init())
			throw AllegroException("Failed to initialize allegro!");

		display_ = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
		if(!display_)
			throw AllegroException("Failed to create display!");

		eventQueue_ = al_create_event_queue();
		if(!eventQueue_)
			throw AllegroException("Failed to create event_queue!");
		
		if(!al_init_primitives_addon())
			throw AllegroException("Failed to initialize addons!");

		if (!al_init_image_addon())
			throw AllegroException("Failed to initialize addons!");

		al_init_font_addon();

		font_ = al_load_bitmap_font((boost::filesystem::path(__FILE__).remove_filename().parent_path() / "a4_font.tga").string().c_str());
		if(!font_)
			throw AllegroException("Failed to initialize font!");

		if(!al_install_keyboard())
			throw AllegroException("Failed to install keyboard!");

		if(!al_install_mouse())
			throw AllegroException("Failed to install mouse!");

		al_set_window_title(display_, "ZPR::Symulator");
		al_register_event_source(eventQueue_, al_get_display_event_source(display_));
		al_register_event_source(eventQueue_, al_get_mouse_event_source());
		al_register_event_source(eventQueue_, al_get_keyboard_event_source());

		static ALLEGRO_COLOR BUTTON_COLOR	= al_map_rgb(110, 190, 230);
		static ALLEGRO_COLOR MENU_BCG_COLOR	= al_map_rgb(100, 100, 100);

		int xMargin = 20, yMargin = 20, height = 40;
		int yOffset = yMargin;
		menuArea = TextRectangle(VISUALISATION_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT, MENU_BCG_COLOR);
		startButton = TextRectangle(VISUALISATION_WIDTH + xMargin, yOffset, WINDOW_WIDTH - xMargin, yOffset + height, BUTTON_COLOR, "Start");
		yOffset += height + yMargin;
		stopButton = TextRectangle(VISUALISATION_WIDTH + xMargin, yOffset, WINDOW_WIDTH - xMargin, yOffset + height, BUTTON_COLOR, "Pause");
		yOffset += height + yMargin;
		restartButton = TextRectangle(VISUALISATION_WIDTH + xMargin, yOffset, WINDOW_WIDTH - xMargin, yOffset + height, BUTTON_COLOR, "Restart");
		yOffset += height + yMargin;
		loopButton = TextRectangle(VISUALISATION_WIDTH + xMargin, yOffset, WINDOW_WIDTH - xMargin, yOffset + height, BUTTON_COLOR, "Loop");
		yOffset += height + yMargin;
		exitButton = TextRectangle(VISUALISATION_WIDTH + xMargin, yOffset, WINDOW_WIDTH - xMargin, yOffset + height, BUTTON_COLOR, "Finish");
	}

	void View::closeGraphics()
	{
		al_uninstall_mouse();
		al_uninstall_keyboard();
		al_unregister_event_source(eventQueue_, al_get_display_event_source(display_));//
		al_destroy_font(font_);
		al_shutdown_font_addon();
		al_shutdown_image_addon();
		al_shutdown_primitives_addon();

		if (eventQueue_)
			al_destroy_event_queue(eventQueue_);
		if (display_)
			al_destroy_display(display_);
		al_uninstall_system();
	}

	void View::scheduleRefresh(long long int simulationTime, long long int elapsed_microseconds)
	{
		{
			boost::lock_guard<boost::mutex> lock(refreshMutex_);
			elapsedMicroseconds_ = elapsed_microseconds;
		}
		refreshCondition_.notify_one();
	}

	void View::operator()()
	{
		try
		{
			initializeGraphics();
			
			ALLEGRO_EVENT allegroEvent;
			bool run = true;

			refresh();

			while(run && !boost::this_thread::interruption_requested())
			{
				// Waiting for refresh signal
				{
					boost::unique_lock<boost::mutex> lock(refreshMutex_);
					//while(elapsedMicroseconds_ < 0)
						refreshCondition_.timed_wait(lock, boost::posix_time::milliseconds(ALLEGRO_EVENT_TIMEOUT));

					if(elapsedMicroseconds_ >= 0)
					{
						elapsedMicroseconds_ = -1;
						refresh();
					}
				}
				
				// Allegro event queue checking
				while(al_get_next_event(eventQueue_, &allegroEvent))
				{
					if(allegroEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
					{
						//controller_.scheduleEvent(boost::make_shared<EventClose>());
						run = false;
					}
					else if(allegroEvent.type == ALLEGRO_EVENT_KEY_UP)
					{
						if(allegroEvent.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
						{
							//controller_.scheduleEvent(boost::make_shared<EventClose>());
							run = false;
						}
					}
					else if(allegroEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
					{
						Point p(allegroEvent.mouse.x, allegroEvent.mouse.y);
						if(startButton.inside(p))
							controller_.scheduleEvent(boost::make_shared<EventStart>());
						else if(stopButton.inside(p))
							controller_.scheduleEvent(boost::make_shared<EventStop>());
						else if(restartButton.inside(p))
							controller_.scheduleEvent(boost::make_shared<EventRestart>());
						else if(exitButton.inside(p))
						{
							//controller_.scheduleEvent(boost::make_shared<EventClose>());
							run = false;
						}
						else if(loopButton.inside(p))
							controller_.scheduleEvent(boost::make_shared<EventLoop>());
					}
				}
			}
		}
		catch(zpr::AllegroException &e)
		{
			Logger::getInstance().error(e.what());
		}
		catch(boost::thread_interrupted)
		{
			// thread interruption
			// closes gracefully
		}
		catch(...)
		{
			Logger::getInstance().error("Unknown View exception.");
		}
		closeGraphics();
		controller_.scheduleEvent(boost::make_shared<EventClose>());
	}

	void View::refresh()
	{
		Model::PGraph g = model_.streets();
		ALLEGRO_TRANSFORM rotate_transformation;

		static ALLEGRO_COLOR WALKER_COLOR		= al_map_rgb(200, 53, 206);
		static ALLEGRO_COLOR BIGCAR_COLOR		= al_map_rgb(0, 53, 206);
		static ALLEGRO_COLOR SMALLCAR_COLOR		= al_map_rgb(255, 126, 0);
		static ALLEGRO_COLOR CAMERA_COLOR		= al_map_rgba(200,0,0,10);
		static ALLEGRO_COLOR GROUND_COLOR		= al_map_rgb(0,100,0);
		static ALLEGRO_COLOR STREETS_COLOR		= al_map_rgb(200, 200, 200);

		al_clear_to_color(GROUND_COLOR);

		for (Graph::MVertices::const_iterator it = g->vertices_.begin(); it != g->vertices_.end(); ++it)
		{
			double w = 10 * SCALER;
			for (Graph::MVertices::const_iterator i = it->second->edges_.begin(); i != it->second->edges_.end(); ++i)
			{
				al_draw_line(it->second->position_.x_ * SCALER, it->second->position_.y_ * SCALER,
								i->second->position_.x_ * SCALER, i->second->position_.y_ * SCALER, STREETS_COLOR, w);
			}
			al_draw_filled_circle(it->second->position_.x_ * SCALER, it->second->position_.y_ * SCALER, w / 2, STREETS_COLOR);
		}

		Model::DTObject objects = model_.objects();
		for (Model::DTObject::const_iterator it = objects.begin(); it != objects.end(); ++it)
		{
			al_identity_transform(&rotate_transformation);
			al_translate_transform(&rotate_transformation, -it->get<0>().x_ * SCALER, -it->get<0>().y_ * SCALER);
			al_rotate_transform(&rotate_transformation, it->get<1>());
			al_translate_transform(&rotate_transformation, it->get<0>().x_ * SCALER, it->get<0>().y_ * SCALER);
			al_use_transform(&rotate_transformation);

			switch (it->get<2>())
			{
				case SMALLCAR: al_draw_filled_rectangle((it->get<0>().x_ - 1.5) * SCALER, (it->get<0>().y_ - 1) * SCALER,
														(it->get<0>().x_ + 1.5) * SCALER, (it->get<0>().y_ + 1) * SCALER,
														SMALLCAR_COLOR);
					break;
				case BIGCAR: al_draw_filled_rectangle((it->get<0>().x_ - 2) * SCALER, (it->get<0>().y_ - 1.5) * SCALER,
														(it->get<0>().x_ + 2) * SCALER, (it->get<0>().y_ + 1.5) * SCALER,
														BIGCAR_COLOR);
					break;
				case WALKER: al_draw_filled_triangle((it->get<0>().x_ - 1) * SCALER, (it->get<0>().y_ - 1) * SCALER,
													(it->get<0>().x_ + 3) * SCALER, it->get<0>().y_ * SCALER,
													(it->get<0>().x_ - 1) * SCALER, (it->get<0>().y_ + 1) * SCALER,
													WALKER_COLOR);
					break;
				default:
					break;
			}			
			al_identity_transform(&rotate_transformation);
			al_use_transform(&rotate_transformation);
			al_draw_text(font_, al_map_rgb(0, 0, 0), it->get<0>().x_ * SCALER, (it->get<0>().y_ + 3) * SCALER, ALLEGRO_ALIGN_CENTRE, it->get<3>().c_str());
			al_draw_text(font_, al_map_rgb(0, 0, 0), it->get<0>().x_ * SCALER, (it->get<0>().y_ + 6) * SCALER, ALLEGRO_ALIGN_CENTRE, boost::lexical_cast<std::string>(boost::format("%6.3f") % it->get<4>()).c_str());
		}

		Dispatcher::DTCamera cameras = model_.cameras();
		for (Dispatcher::DTCamera::const_iterator it = cameras.begin(); it != cameras.end(); ++it)
		{
			al_draw_filled_circle(it->get<0>().x_ * SCALER, it->get<0>().y_ * SCALER, 2 * SCALER, CAMERA_COLOR);

			al_draw_filled_pieslice(it->get<0>().x_ * SCALER, it->get<0>().y_ * SCALER, it->get<1>() * SCALER,
									it->get<2>() - it->get<3>() / 2, it->get<3>(),
									CAMERA_COLOR);
			al_draw_text(font_, al_map_rgb(0, 0, 0), it->get<0>().x_ * SCALER, (it->get<0>().y_ + 3) * SCALER, ALLEGRO_ALIGN_CENTRE, ("Camera: " + boost::lexical_cast<std::string>(it->get<4>())).c_str());
		}

		drawMenu();
		al_flip_display();
	}

	void View::drawMenu()
	{
		static ALLEGRO_COLOR FONT_COLOR		= al_map_rgb(255, 255, 255);
		menuArea.draw();
		startButton.draw(font_, FONT_COLOR);
		stopButton.draw(font_, FONT_COLOR);
		restartButton.draw(font_, FONT_COLOR);
		loopButton.draw(font_, FONT_COLOR);
		exitButton.draw(font_, FONT_COLOR);
	}
}