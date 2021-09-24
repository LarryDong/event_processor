# event_processor
Events processing codes

## show_data
```bash
rosrun show_data show_data_node --<flags>=<values>

# example:
# based on ts_file:
rosrun show_data show_data_node --event_file=events.txt --ts_file=image_ts.txt
# not based on ts, but a fixed value:
rosrun show_data show_data_node --event_file=events.txt --ts_from_file=false --time_duration=30
```
@flags:
    --event_file: path of `events.txt`  
    --ts_from_file: seperate events based on `ts_file` or not. value: true/false  
    --ts_file: `image_ts.txt` which contains all ts from a rgb camera  
    --time_duration: fixed value for drawing events if `ts_from_file` is `false`. unit: us  