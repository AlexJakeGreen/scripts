package ua.kh.bars.service;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.stream.Collectors;
import ua.kh.bars.dao.EventDAO;
import ua.kh.bars.entity.Auditorium;
import ua.kh.bars.entity.Event;
import ua.kh.bars.entity.EventRating;

public class EventService {

    private EventDAO eventDAO;

    public EventService() {
    }
    
    public EventService(EventDAO eventDAO) {
        this.eventDAO = eventDAO;
    }

    public Iterable<Event> getAll() {
        return eventDAO.findAll();
    }

    public Event create(Date date, String name, int price, EventRating rating) {
        Event event = new Event(date, name, price, rating);
        eventDAO.save(event);
        return event;
    };

    public void remove(Event event) {
        eventDAO.delete(event);
    };

    public Event getById(Date id) {
        return eventDAO.findOne(id);
    };

    public Iterable<Event> getByName(String name) {
        return eventDAO.findByName(name);
    };

    public Iterable<Event> getForDateRange(Date from, Date to) {
        return eventDAO.findAll().stream()
            .filter( it -> it.getDate() != null
                     && it.getDate().compareTo(from) > 0
                     && it.getDate().compareTo(to) < 0)
            .collect(Collectors.toList());
    }

    public Iterable<Event> getNextEvents(Date to) {
        return getForDateRange(new Date(), to);
    }

    public void assignAuditorium(Event event,
                                 Auditorium auditorium, Date date) {
        event.setAuditorium(auditorium);
        event.setDate(date);
    }
}
