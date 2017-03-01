package ua.kh.bars.services;

import ua.kh.bars.entities.Event;

import java.util.List;

/**
 * Created by green on 3/18/16.
 */
public interface EventService {

    List<Event> findAll();
    Event findOne(Long id);
    void create(String name, Long auditoriumId, int price);

}
