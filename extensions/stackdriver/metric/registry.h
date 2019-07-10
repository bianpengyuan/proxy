/* Copyright 2019 Istio Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "extensions/stackdriver/common/context.h"
#include "opencensus/exporters/stats/stackdriver/stackdriver_exporter.h"
#include "opencensus/stats/measure.h"
#include "opencensus/stats/stats.h"
#include "opencensus/stats/tag_key.h"

namespace Stackdriver {
namespace Metric {

// Returns Stackdriver exporter config option based on node metadata.
opencensus::exporters::stats::StackdriverOptions
GetStackdriverOptions(const Stackdriver::Common::NodeInfo &local_node_info);

// Registers Opencensus views
void RegisterViews();

// Opencensus tag key functions.
opencensus::tags::TagKey RequestOperationKey();
opencensus::tags::TagKey RequestProtocolKey();
opencensus::tags::TagKey ServiceAuthenticationPolicyKey();
opencensus::tags::TagKey MeshUIDKey();
opencensus::tags::TagKey DestinationServiceNameKey();
opencensus::tags::TagKey DestinationServiceNamespaceKey();
opencensus::tags::TagKey DestinationPortKey();
opencensus::tags::TagKey ResponseCodeKey();
opencensus::tags::TagKey SourcePrincipalKey();
opencensus::tags::TagKey SourceWorkloadNameKey();
opencensus::tags::TagKey SourceWorkloadNamespaceKey();
opencensus::tags::TagKey SourceOwnerKey();
opencensus::tags::TagKey DestinationPrincipalKey();
opencensus::tags::TagKey DestinationWorkloadNameKey();
opencensus::tags::TagKey DestinationWorkloadNamespaceKey();
opencensus::tags::TagKey DestinationOwnerKey();

// Opencensus measure functions.
opencensus::stats::MeasureInt64 ServerRequestCountMeasure();
opencensus::stats::MeasureInt64 ServerRequestBytesMeasure();
opencensus::stats::MeasureInt64 ServerResponseBytesMeasure();
opencensus::stats::MeasureDouble ServerResponseLatenciesMeasure();
opencensus::stats::MeasureInt64 ClientRequestCountMeasure();
opencensus::stats::MeasureInt64 ClientRequestBytesMeasure();
opencensus::stats::MeasureInt64 ClientResponseBytesMeasure();
opencensus::stats::MeasureDouble ClientRoundtripLatenciesMeasure();

} // namespace Metric
} // namespace Stackdriver
