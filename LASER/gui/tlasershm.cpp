#include "tlasershm.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>

#include <unistd.h>



TLaserShm::TLaserShm(QObject *parent) :
    QObject(parent)
{
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shm_fd, SHM_SIZE);
    shm_content = (shm_content_t *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_content == MAP_FAILED)
        std::cerr << "mmap() Failed." << std::endl;

}

void TLaserShm::ustawCzasImpulsu(int pwm_work)
{
    shm_content->pwm_work = pwm_work;
}
