//
// Created by badaccuracyid on 12/11/22.
//

#ifndef GNINIARTERPSD_UTILS_H
#define GNINIARTERPSD_UTILS_H


class Utils {

public:
    static unsigned long djb2_hash(const char *str);

    static char * getFormatteDate();
};


#endif //GNINIARTERPSD_UTILS_H
