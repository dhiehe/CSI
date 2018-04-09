#ifndef ROAMROUTE_H
#define ROAMROUTE_H

#include <vector>
using namespace std;


#ifndef ROAM_STATUS
#define ROAM_STATUS

#define ROAM_STATUS_IDLE 0
#define ROAM_STATUS_READY 1
#define ROAM_STATUS_RUNNING 2
#define ROAM_STATUS_PAUSE 3

#define ROAMX(roam, index) (roam->roam_positions_x[index])
#define ROAMY(roam, index) (roam->roam_positions_y[index])

#endif // ROAM_STATUS

class RoamRoute
{
public:
    RoamRoute();
    bool addPos(int x, int y);
    void setPos(int index, int x, int y);
    void dropPos(int index);
    void nextPos();
    int status();

    void activate();
    void run();
    void pause();
    void stop();
    void clear();
    int size();

    vector<int> roam_positions_x;
    vector<int> roam_positions_y;
    int current_index;

private:
    int tail;
    int roam_status;
};

#endif // ROAMROUTE_H
