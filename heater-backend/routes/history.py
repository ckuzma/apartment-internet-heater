from flask import request
from flask_restful import Resource
import json

## Import our API components
from api.historian import Historian
from config import Config

_config = Config()

class HistorianView(Resource):
    def __init__(self):
        self._classify_logic = HistorianLogic()

    def get(self):
        return self._classify_logic.get_history()


class HistorianLogic:
    def __init__(self):
        self._historian = Historian(_config.credentials['iotf']['username'], _config.credentials['iotf']['password'], _config.credentials['iotf']['org'])

    def get_history(self):
        history = self._historian.get()
        return history


"""
===================
----- Tests -------
===================
"""
#
# _historian = HistorianLogic()
# _historian.get_history()
