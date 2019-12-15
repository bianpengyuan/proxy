# Copyright 2019 Istio Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

  
.PHONY: test_stackdriver.build_and_push

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
SD_PATH := $(dir $(MKFILE_PATH))

test_stackdriver.build_and_push:
	cd $(SD_PATH) && go build main.go
	docker build $(SD_PATH) -t gcr.io/istio-testing/test-stackdriver:1.0
	docker push gcr.io/istio-testing/test-stackdriver:1.0
	rm $(SD_PATH)/main

