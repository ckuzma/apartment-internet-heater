#! /usr/bin/env python3
from flask import Flask, request
from flask_restful import Resource, Api
import os

## Import our API components
from routes.history import HistorianView

app = Flask(__name__)
api = Api(app)

## List all the routes we want here:
api.add_resource(HistorianView, '/history')

port = os.getenv('PORT', 5000)
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=int(port), debug=False) # Turn debug on for more verbose output
