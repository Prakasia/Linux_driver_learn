def main():
    driver_handle = open('/proc/study_driver')
    msg_from_kernel = driver_handle.readline()
    print(msg_from_kernel)
    driver_handle.close()
    return
main()