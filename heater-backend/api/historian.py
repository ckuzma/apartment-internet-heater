from datetime import datetime
import json
import requests
from time import strftime

HISTORIAN_URL = "https://internetofthings.ibmcloud.com/api/v0001/historian/"

class Historian:
    def __init__(self, username, password, org_id):
        self.username = username
        self.password = password
        self.org = org_id

    def get(self):
        url = HISTORIAN_URL + self.org
        response = requests.get(url, auth=(self.username, self.password))
        return json.loads(response.text)
