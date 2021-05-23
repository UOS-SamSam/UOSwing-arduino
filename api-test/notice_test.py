from samsam_api import Samsam_api

def main():
    email = "samsam-uos@gmail.com"
    password = "33wing"
    samsam_api = Samsam_api()
    samsam_api.login(email, password)

    notices = samsam_api.get_all_notice()
    for notice in notices:
        print(notice)

    notice_id = samsam_api.post_notice("공지 테스트 내용", "공지 테스트 제목")
    samsam_api.get_notice(notice_id)
    samsam_api.patch_notice(notice_id, "공지 테스트 수정 내용", "공지 테스트 수정 제목")
    samsam_api.delete_notice(notice_id)


if __name__ == '__main__':
    main()

