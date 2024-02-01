#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

int main(int argc, char* argv[])
{
    Fl_Window* window = new Fl_Window(1080, 720);
    window->fullscreen();
    Fl_Box* label = new Fl_Box(20, 0, 200, 100, "hello, world!");
    label->box(FL_FLAT_BOX);
    label->labelfont(FL_BOLD + FL_ITALIC);
    label->labelsize(36);
    label->labeltype(FL_SHADOW_LABEL);
    window->end();
    window->show(argc, argv);
    return Fl::run();
}
