from flask import Flask
from Webapp.views.index import bp as index_bp

app = Flask(__name__)

app.register_blueprint(index_bp)