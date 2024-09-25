#include <vapplication.h>

using namespace vgcl;

VApplication app;

int main(int argc, char* argv[])
{
    app.init(argc, argv);
    return app.run();
}
