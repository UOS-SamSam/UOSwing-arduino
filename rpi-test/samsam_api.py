import requests

class Samsam_api:
    def __init__(self):
        self.url = 'http://api.wing-test.kro.kr:8080'
        self.x_auth_token = None

    def login(self, email, password):
        uri = self.url + '/api/v1/admin/login'
        ret = requests.post(uri, json={'email': email, 'password': password})
        print(uri)
        if ret.status_code == 200:
            print("login success")
            self.x_auth_token = ret.text
        else:
            print('login error:', ret.status_code)
            return

    # padBoxLog
    def get_statistics(self, duration):
        uri = self.url + '/api/v1/statistics/' + str(duration)
        ret = requests.get(uri, headers={'X-Auth-Token': self.x_auth_token})
        if ret.status_code != 200:
            print('statistics get error:', ret.status_code)
            return
        return ret.json()

    # padBox
    def get_all_padbox(self):
        uri = self.url + '/api/v1/padbox/'
        ret = requests.get(uri)
        if ret.status_code != 200:
            print('padbox get error:', ret.status_code)
            return
        return ret.json()

    def get_padbox(self, id):
        uri = self.url + '/api/v1/padbox/' + str(id)
        ret = requests.get(uri)
        if ret.status_code != 200:
            print('padbox get error:', ret.status_code)
            return
        return ret.json()

    def post_padbox(self, address, humidity, latitude, longitude, name, padAmount, temperature):
        uri = self.url + '/api/v1/padbox/'
        ret = requests.post(uri, headers={'X-Auth-Token': self.x_auth_token},
                            json={
                                "address": address,
                                "humidity": humidity,
                                "latitude": latitude,
                                "longitude": longitude,
                                "name": name,
                                "padAmount": padAmount,
                                "temperature": temperature
                            })
        if ret.status_code != 200:
            print('padbox post error:', ret.status_code)
            return -1
        else:
            print('padbox post success')
            return int(ret.text)

    def delete_padbox(self, id):
        uri = self.url + '/api/v1/padbox/' + str(id)
        ret = requests.delete(uri, headers={'X-Auth-Token': self.x_auth_token})
        if ret.status_code != 200:
            print('padbox delete error:', ret.status_code)
            return
        else:
            print('padbox delete success')

    def patch_padbox(self, id, address, latitude, longitude, name):
        uri = self.url + '/api/v1/padbox/' + str(id)
        ret = requests.patch(uri, headers={'X-Auth-Token': self.x_auth_token},
                            json={
                                "address": address,
                                "latitude": latitude,
                                "longitude": longitude,
                                "name": name,
                            })
        if ret.status_code != 200:
            print('padbox patch error:', ret.status_code)
            return
        else:
            print('padbox patch success')

    def patch_padbox_state(self, id, humidity, padAmount, temperature):
        uri = self.url + '/api/v1/padbox/updateState/' + str(id)
        ret = requests.patch(uri, headers={'X-Auth-Token': self.x_auth_token},
                            json={
                                "humidity": humidity,
                                "padAmount": padAmount,
                                "temperature": temperature
                            })
        if ret.status_code != 200:
            print('padbox patch state error:', ret.status_code)
            return
        else:
            print('padbox patch state success')

    # report
    def get_all_report(self):
        uri = self.url + '/api/v1/report/'
        ret = requests.get(uri, headers={'X-Auth-Token': self.x_auth_token})
        if ret.status_code != 200:
            print('report get error:', ret.status_code)
            return
        return ret.json()

    def get_report(self, id):
        uri = self.url + '/api/v1/report/' + str(id)
        ret = requests.get(uri, headers={'X-Auth-Token': self.x_auth_token})
        if ret.status_code != 200:
            print('report get error:', ret.status_code)
            return
        return ret.json()

    def post_report(self, content, isResolved, padboxId, tag):
        uri = self.url + '/api/v1/report/'
        ret = requests.post(uri, headers={'X-Auth-Token': self.x_auth_token},
                            json={
                                "content": content,
                                "isResolved": isResolved,
                                "padBoxId": padboxId,
                                "tag": tag
                            })
        if ret.status_code != 200:
            print('report post error:', ret.status_code)
            return -1
        else:
            print('report post success')
            return int(ret.text)

    def delete_report(self, id):
        uri = self.url + '/api/v1/report/' + str(id)
        ret = requests.delete(uri, headers={'X-Auth-Token': self.x_auth_token})
        if ret.status_code != 200:
            print('report delete error:', ret.status_code)
            return
        else:
            print(f'report delete success: id={id}')

    def patch_report(self, id, isResolved):
        uri = self.url + '/api/v1/report/' + str(id)
        ret = requests.patch(uri, headers={'X-Auth-Token': self.x_auth_token},
                             json={
                                 "isResolved": isResolved
                             })
        if ret.status_code != 200:
            print('report patch error:', ret.status_code)
            return
        else:
            print(f'report patch success: id={id}')

    # notice
    def get_all_notice(self):
        uri = self.url + '/api/v1/notice/'
        ret = requests.get(uri, headers={'X-Auth-Token': self.x_auth_token})
        if ret.status_code != 200:
            print('notice get error:', ret.status_code)
            return
        return ret.json()

    def get_notice(self, id):
        uri = self.url + '/api/v1/notice/' + str(id)
        ret = requests.get(uri, headers={'X-Auth-Token': self.x_auth_token})
        if ret.status_code != 200:
            print('notice get error:', ret.status_code)
            return
        return ret.json()

    def post_notice(self, content, title):
        uri = self.url + '/api/v1/notice/'
        ret = requests.post(uri, headers={'X-Auth-Token': self.x_auth_token},
                            json={
                                "content": content,
                                "title": title,
                            })
        if ret.status_code != 200:
            print('notice post error:', ret.status_code)
            return -1
        else:
            print('notice post success')
            return int(ret.text)

    def delete_notice(self, id):
        uri = self.url + '/api/v1/notice/' + str(id)
        ret = requests.delete(uri, headers={'X-Auth-Token': self.x_auth_token})
        if ret.status_code != 200:
            print('notice delete error:', ret.status_code)
            return
        else:
            print(f'notice delete success: id={id}')

    def patch_notice(self, id, content, title):
        uri = self.url + '/api/v1/notice/' + str(id)
        ret = requests.patch(uri, headers={'X-Auth-Token': self.x_auth_token},
                             json={
                                 "content": content,
                                 "title": title
                             })
        if ret.status_code != 200:
            print('notice patch error:', ret.status_code)
            return
        else:
            print(f'notice patch success: id={id}')