#!/bin/sh
sudo perf record --call-graph dwarf ../out/release/crack-tracer -f -o per.data

