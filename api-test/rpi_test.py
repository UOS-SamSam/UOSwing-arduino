from samsam_api import Samsam_api

def main():
    email = "samsam-uos@gmail.com"
    password = "33wing"
    samsam_api = Samsam_api()
    samsam_api.login(email, password)

    padboxs = samsam_api.get_all_padbox()
    for padbox in padboxs:
        print(padbox)
	# samsam_api.patch_padbox(padbox_id, "padbox 테스트 수정", 1.1, 1.1, "padbox 테스트 이름 수정")

if __name__ == '__main__':
    main()

