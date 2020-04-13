// Copyright 2019 Istio Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package client_test

import (
	"testing"

	"istio.io/proxy/test/envoye2e"
	"istio.io/proxy/test/envoye2e/driver"
)

func TestBasicFlow(t *testing.T) {
	params := driver.NewTestParams(t, map[string]string{
		"EnableEchoBackend": "true",
		"RequestCount":      "10",
	}, envoye2e.ProxyE2ETests)
	if err := (&driver.Scenario{
		[]driver.Step{
			&driver.XDS{},
			&driver.ClientServerEnvoy{},
			&driver.Repeat{
				N: 10,
				Step: &driver.HTTPCall{
					Port: params.Ports.ClientPort,
					Path: "/echo",
				},
			},
			&driver.Stats{
				AdminPort: params.Ports.ServerAdmin,
				Matchers: map[string]driver.StatMatcher{
					"envoy_listener_http_downstream_rq_xx": &driver.PartialStat{Metric: "testdata/metric/basic_flow_server_requests.yaml.tmpl"},
				}},
			&driver.Stats{
				AdminPort: params.Ports.ClientAdmin,
				Matchers: map[string]driver.StatMatcher{
					"envoy_listener_http_downstream_rq_xx": &driver.PartialStat{Metric: "testdata/metric/basic_flow_client_requests.yaml.tmpl"},
				}},
		},
	}).Run(params); err != nil {
		t.Fatal(err)
	}
}
