#ifndef NTFY_SERVICE_H
#define NTFY_SERVICE_H

#include <Arduino.h>

class NtfyService {
public:
    NtfyService(const char* topic);

    bool sendNotification(
        const char* title,
        const char* message,
        const char* priority,
        const char* tags
    );

private:
    const char* _topic;
};

#endif