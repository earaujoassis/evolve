from celery import Celery

app = Celery('tasks')
app.config_from_object('conf.celeryconfig')

@app.task
def run_experiment(e_data):
    return e_data
