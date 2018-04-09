#include "roamroute.h"

RoamRoute::RoamRoute()
{
    current_index = -1;
    tail = -1;
    roam_status = ROAM_STATUS_IDLE;
}

/* if too close to last one, or out of range,
 * change the last one.
 * else,
 * add to roamroute.
 */
bool RoamRoute::addPos(int x, int y)
{
    if (tail>=9 ||
        (tail>=0 && (ROAMX(this, tail)-x)*(ROAMX(this, tail)-x)+(ROAMY(this, tail)-y)*(ROAMY(this, tail)-y)<100)
       ) {
        this->setPos(tail, x, y);
        return false;
    }
    else {
        roam_positions_x.push_back(x);
        roam_positions_y.push_back(y);
        tail++;
        return true;
    }
}

void RoamRoute::setPos(int index, int x, int y)
{
    if (index>=0 && index<=tail) {
        ROAMX(this, index) = x;
        ROAMY(this, index) = y;
    }
}

void RoamRoute::dropPos(int index)
{
    if (index>=0 && index<=tail) {
        roam_positions_x.erase(roam_positions_x.begin()+index);
        roam_positions_y.erase(roam_positions_y.begin()+index);
        tail--;
    }
}

void RoamRoute::nextPos()
{
    if (roam_status == ROAM_STATUS_RUNNING) {
        current_index = current_index==tail ? 0:
                                              current_index + 1;
    }
}

int RoamRoute::status()
{
    return roam_status;
}

//void activate();
//void run();
//void pause();
//void stop();

/* IDLE -> READY
 */
void RoamRoute::activate()
{
    // if empty, deny
    if (tail<1) {
        return;
    }
    if (roam_status == ROAM_STATUS_IDLE) {
        roam_status = ROAM_STATUS_READY;
        current_index = 0;
    }
}

/* READY -> RUNNING
 * PAUSE -> RUNNING
 */
void RoamRoute::run()
{
    if (roam_status == ROAM_STATUS_READY || roam_status == ROAM_STATUS_PAUSE) {
        roam_status = ROAM_STATUS_RUNNING;
    }
}

/* RUNNING -> PAUSE
 */
void RoamRoute::pause()
{
    if (roam_status == ROAM_STATUS_RUNNING) {
        roam_status = ROAM_STATUS_PAUSE;
    }
}

/* READY -> IDLE
 * RUNNING -> IDLE
 * PAUSE -> IDLE
 */
void RoamRoute::stop()
{
    if (roam_status == ROAM_STATUS_READY || roam_status == ROAM_STATUS_RUNNING || roam_status == ROAM_STATUS_PAUSE) {
        roam_status = ROAM_STATUS_IDLE;
    }
}

/*
 * only when IDLE or READY
 */
void RoamRoute::clear()
{
    if (roam_status == ROAM_STATUS_IDLE || roam_status == ROAM_STATUS_READY) {
        roam_positions_x.clear();
        roam_positions_y.clear();
        current_index = -1;
        tail = -1;
        roam_status = ROAM_STATUS_IDLE;
    }
}

/*
 * return position amount
 */
int RoamRoute::size()
{
    return tail+1;
}
