from setuptools import find_packages, setup
from glob import glob
import os

package_name = 'arduinobot_vision'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(include=['arduinobot_vision', 'arduinobot_vision.*']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob('launch/*.py')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='farr97',
    maintainer_email='farr97@todo.todo',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'realsense_node = arduinobot_vision.scripts.realsense_node:main',
        ],
    },
)
