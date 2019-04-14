#ifndef PHIL_H
#define PHIL_H

class Phil {
    private:
        int id; // which philosopher
        bool leftAvailable, rightAvailable; // access to files on left and right

    public:        
        Phil() {
            id = -1;
            leftAvailable = false;
            rightAvailable = false;
        }

        Phil(int nId, bool lAvail, bool rAvail) {
            id = nId;
            leftAvailable = lAvail;
            rightAvailable = rAvail;
        }

        int getID();
        bool isLeftAvailable();
        bool isRightAvailable();

        void setLeftAvailable(const bool is_available);
        void setRightAvailable(const bool is_available);
};

#endif