import os
import yaml

class Config:
    def __init__(self):
        self.credentials = self._read_creds()

    def _read_creds(self):
        try:
            raw = open('config/creds.yaml', 'r')
            creds = yaml.load(raw.read())
            raw.close()
            return creds
        except:
            print('\n\tAn error occurred when attempting to read config/creds.yaml\n')


"""
===================
----- Tests -------
===================
"""
# _config = Config()

## Read in the credentials:
# creds = _config._read_creds()
