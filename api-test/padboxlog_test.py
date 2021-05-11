from api import Samsam_api

def main():
    email = "samsam-uos@gmail.com"
    password = "33wing"
    samsam_api = Samsam_api()
    samsam_api.login(email, password)

    print("7일 통계:")
    statistics = samsam_api.get_statistics(7)
    for statistic in statistics:
        print(statistic)

    print("30일 통계:")
    statistics = samsam_api.get_statistics(30)
    for statistic in statistics:
        print(statistic)


if __name__ == '__main__':
    main()

