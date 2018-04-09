#include "scenemode.h"
#include <qDebug>

SceneMode::SceneMode(unsigned int mode)
{
    if (mode>SCENE_MODE_LAST) {
        current_scene_mode = SCENE_MODE_LAST;
    }
    else {
        current_scene_mode = mode;
    }
    former_scene_mode = current_scene_mode;
}

bool SceneMode::is(unsigned int mode)
{
    return (current_scene_mode==mode);
}

void SceneMode::set(unsigned int mode)
{
    if (mode>SCENE_MODE_LAST) {
        /* do nothing */
        // current_scene_mode = SCENE_MODE_LAST;
    }
    else {
        former_scene_mode = current_scene_mode;
        current_scene_mode = mode;
        qDebug() << "SCENEMODE::SET set to " << this->current_scene_mode;
    }

}

void SceneMode::setformer()
{
    int tmp = current_scene_mode;
    current_scene_mode = former_scene_mode;
    former_scene_mode = tmp;
    qDebug() << "SCENEMODE::setformer set to " << this->current_scene_mode;
}

bool SceneMode::formeris(unsigned int mode)
{
    return (former_scene_mode==mode);
}

int SceneMode::print()
{
    /*
    SCENE_MODE_FREEVIEW 0
    SCENE_MODE_ROAMRUN 1
    SCENE_MODE_ROAMPAUSE 2
    SCENE_MODE_CHOOSEDOT 3
    */
    return current_scene_mode;
}
