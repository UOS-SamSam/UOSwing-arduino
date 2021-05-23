from samsam_api import Samsam_api

def main():
    email = "samsam-uos@gmail.com"
    password = "33wing"
    samsam_api = Samsam_api()
    samsam_api.login(email, password)

    padboxs = samsam_api.get_all_padbox()
    for padbox in padboxs:
        print(padbox)

    padbox_id = samsam_api.post_padbox(
        address="padbox 테스트",
        humidity=9999999,
        latitude=0,
        longitude=0,
        name="padbox 테스트 이름",
        padAmount=9999999,
        temperature=99999)
    samsam_api.get_padbox(padbox_id)
    samsam_api.patch_padbox(padbox_id, "padbox 테스트 수정", 1.1, 1.1, "padbox 테스트 이름 수정")
    samsam_api.delete_padbox(padbox_id)


if __name__ == '__main__':
    main()

