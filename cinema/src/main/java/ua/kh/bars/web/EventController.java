package ua.kh.bars.web;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import ua.kh.bars.entities.Event;
import ua.kh.bars.services.EventService;

/**
 * Created by green on 3/19/16.
 */

@Controller
@RequestMapping("/events")
public class EventController {

    @Autowired
    EventService eventService;

    @RequestMapping(method = RequestMethod.GET)
    public String indexEvents(Model model) {
        Iterable<Event> events = eventService.findAll();
        model.addAttribute("events", events);
        return "events/index";
    }

    @RequestMapping(value = "/{eventId}", method = RequestMethod.GET)
    public String showEvent(@PathVariable("eventId") Long eventId, Model model) {
        Event event = eventService.findOne(eventId);
        model.addAttribute("event", event);
        return "events/show";
    }


}
