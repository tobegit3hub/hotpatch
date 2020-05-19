# Upload the package to pypi.
# 1. python ./setup.py bdist_wheel --universal
# 2. twine upload dist/hotpatch-x.x.x-py2.py3-none-any.whl

from setuptools import setup, find_packages

setup(
    name="hotpatch",
    version="0.0.1",
    author="tobe",
    author_email="tobeg3oogle@gmail.com",
    url="https://github.com/tobegit3hub/hotpatch",
    description="Apply hot patches for running processes without restarting",
    packages=find_packages(),
    install_requires=[],
    include_package_data=True,
    zip_safe=False,
    entry_points={
        "console_scripts": [
            "hotpatch=hotpatch.cli.hotpatch_cli:main"
        ]
    }
)
