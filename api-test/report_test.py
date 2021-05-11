from api import Samsam_api

def main():
    email = "samsam-uos@gmail.com"
    password = "33wing"
    samsam_api = Samsam_api()
    samsam_api.login(email, password)

    reports = samsam_api.get_all_report()
    for report in reports:
        print(report)

    report_id = samsam_api.post_report("신고 테스트", False, 2, "KEY_MISSED")
    samsam_api.patch_report(report_id, True)
    samsam_api.delete_report(report_id)


if __name__ == '__main__':
    main()

