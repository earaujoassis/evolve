from celeryconfig_local import *

CELERY_TASK_SERIALIZER = 'json'
CELERY_RESULT_SERIALIZER = 'json'
CELERY_ACCEPT_CONTENT=['json']
CELERY_TIMEZONE = 'America/Sao_Paulo'
CELERY_ENABLE_UTC = True
