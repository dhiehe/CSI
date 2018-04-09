#ifndef SCENEMODE_H
#define SCENEMODE_H

#define SCENE_MODE_FREEVIEW 0
#define SCENE_MODE_ROAMRUN 1
#define SCENE_MODE_ROAMPAUSE 2
#define SCENE_MODE_CHOOSEDOT 3
#define SCENE_MODE_HOTSPOT 4

#define SCENE_MODE_LAST SCENE_MODE_HOTSPOT

class SceneMode
{
public:
    SceneMode(unsigned int mode=SCENE_MODE_FREEVIEW);
    bool is(unsigned int mode);
    void set(unsigned int mode);
    void setformer();
    bool formeris(unsigned int mode);
    int print();

private:
    int former_scene_mode;
    int current_scene_mode;
};

#endif // SCENEMODE_H
